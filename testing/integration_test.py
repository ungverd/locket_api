import unittest
import time
import serial

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

    @classmethod
    def tearDownClass(cls):
        del cls.localUart

    def test_01_JoiningNetwork(self):
        self.localUart.waitUntilStringInUart('30 seconds passed')


if __name__ == '__main__':
    unittest.main()
