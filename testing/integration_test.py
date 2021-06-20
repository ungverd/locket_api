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

    def waitUntilStringInUart(self, waitFor):
        while True:
            line = self.connection.readline().decode('ascii', 'ignore')
            if line:
                print('Received: ' + line, end='')
                if waitFor in line:
                    return
            time.sleep(0.1)


class TestLightIsDiscoverable(unittest.TestCase):
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

    def testPrimaryDeviceIsOnline(self):
        self.primaryUart.waitUntilStringInUart('Started execution')

    def testSecondaryDeviceIsOnline(self):
        self.secondaryUart.waitUntilStringInUart('Started execution')


if __name__ == '__main__':
    unittest.main()

