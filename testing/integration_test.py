import unittest
import time
from typing import Optional

import serial
import subprocess

from serial.tools import list_ports


PRIMARY_DEVICE_ID = '0672FF555550877067141642'
SECONDARY_DEVICE_ID = '066FFF534953867067205729'


def comPortName(device_id: str) -> Optional[str]:
    for port in list_ports.comports():
        if port.serial_number == device_id:
            return port.name
    return None


def buildAndUpload(hla: str):
    out = subprocess.check_output(['openocd',
                                   '-f', 'embedded/openocd.cfg',
                                   '-c', 'hla_serial "%s"' % hla,
                                   '-c', 'tcl_port disabled',
                                   '-c', 'gdb_port disabled',
                                   '-c', 'program "cmake-build-debug-arm-native/projects/integration_testing_behavior.elf"',
                                   '-c', 'init',
                                   '-c', 'reset run',
                                   '-c', 'exit'], cwd='..').decode('utf-8')
    print(out)


class UartHelper:
    def __init__(self, port):
        self.connection = serial.Serial(port, 115200)

    def __del__(self):
        self.connection.close()

    def waitUntilStringInUart(self, waitFor: str, timeout: float = 5.0):
        self.connection.timeout = timeout
        deadline = time.time() + timeout
        while time.time() < deadline:
            if self.connection.readable():
                line = self.connection.readline().decode('ascii', 'ignore')
                if line:
                    print('Received: ' + line, end='')
                    if waitFor in line:
                        return
            time.sleep(0.1)
        raise TimeoutError('Timeout while waiting for %s to appear in UART' % waitFor)


class TestLocketHardwareLibraries(unittest.TestCase):
    primaryUart = None
    secondaryUart = None

    @classmethod
    def setUpClass(cls):
        cls.primaryUart = UartHelper(comPortName(PRIMARY_DEVICE_ID))
        cls.secondaryUart = UartHelper(comPortName(SECONDARY_DEVICE_ID))
        buildAndUpload(PRIMARY_DEVICE_ID)
        buildAndUpload(SECONDARY_DEVICE_ID)

    @classmethod
    def tearDownClass(cls):
        del cls.primaryUart
        del cls.secondaryUart

    # Next 2 tests have 000 prefix so they are executed first (otherwise other tests will "consume" part of uart
    # log containing "Started execution").
    def test000PrimaryDeviceIsOnline(self):
        self.primaryUart.waitUntilStringInUart('Started execution')

    def test000SecondaryDeviceIsOnline(self):
        self.secondaryUart.waitUntilStringInUart('Started execution')

    def testRespondsToPing(self):
        self.primaryUart.connection.write(b'ping\n')
        self.primaryUart.waitUntilStringInUart('pong')

    def testRespondsToCommandWithArguments(self):
        self.primaryUart.connection.write(b'plus 20 -10\n')
        self.primaryUart.waitUntilStringInUart('sum equals 10')

    def testSimpleRadioCommunication(self):
        self.primaryUart.connection.write(b'emit 1637\n')
        self.secondaryUart.waitUntilStringInUart('received packet with id 1637')

    def testCanWriteAndReadEeprom(self):
        self.primaryUart.connection.write(b'eeprom_write 23 178\n')
        self.primaryUart.waitUntilStringInUart('EEPROM write success')
        self.primaryUart.connection.write(b'eeprom_read 23\n')
        self.primaryUart.waitUntilStringInUart('value in EEPROM: 178')


if __name__ == '__main__':
    unittest.main()

