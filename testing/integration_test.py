import unittest
import time
import serial
import subprocess

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

    @classmethod
    def setUpClass(cls):
        cls.localUart = UartHelper("COM5")
        out = subprocess.check_output(['openocd', '-f', '../embedded/openocd_reset.cfg']).decode('utf-8')
        print(out)

    @classmethod
    def tearDownClass(cls):
        del cls.localUart

    def test_01_JoiningNetwork(self):
        self.localUart.waitUntilStringInUart('Started execution')


if __name__ == '__main__':
    unittest.main()
