#!/usr/bin/env python3

import base64
import os
import subprocess
import unittest
import random


class TestAscii85Compliance(unittest.TestCase):

    def setUp(self):
        self.ascii85_exe = './ascii85'
        if not os.path.exists(self.ascii85_exe):
            self.ascii85_exe = os.path.join(os.path.dirname(__file__), '..', 'build', 'ascii85')
        if not os.path.exists(self.ascii85_exe):
            self.ascii85_exe = os.path.join(os.path.dirname(__file__), '..', 'ascii85')

    def test_a85_encode_matches_python(self):
        """Наше кодирование должно совпадать с base64.a85encode"""
        if not os.path.exists(self.ascii85_exe):
            self.skipTest(f"Исполняемый файл не найден: {self.ascii85_exe}")

        for _ in range(50):
            data = os.urandom(random.randint(1, 4096))
            py_encoded = base64.a85encode(data, foldspaces=False, wrapcol=0, pad=False, adobe=False)

            proc = subprocess.run([self.ascii85_exe, '-eb'], input=data, capture_output=True, text=False)
            self.assertEqual(proc.returncode, 0, f"Кодирование не удалось: {proc.stderr}")
            self.assertEqual(proc.stdout.rstrip(b"\n"), py_encoded, "Наше кодирование должно совпадать с base64.a85encode")

    def test_a85_decode_matches_python(self):
        """Наше декодирование должно совпадать с base64.a85decode"""
        if not os.path.exists(self.ascii85_exe):
            self.skipTest(f"Исполняемый файл не найден: {self.ascii85_exe}")

        for _ in range(50):
            data = os.urandom(random.randint(1, 4096))
            py_encoded = base64.a85encode(data, foldspaces=False, wrapcol=0, pad=False, adobe=False)

            proc = subprocess.run([self.ascii85_exe, '-db'], input=py_encoded, capture_output=True, text=False)
            self.assertEqual(proc.returncode, 0, f"Декодирование не удалось: {proc.stderr}")
            self.assertEqual(proc.stdout, data)

            proc2 = subprocess.run([self.ascii85_exe, '-eb'], input=data, capture_output=True, text=False)
            self.assertEqual(proc2.returncode, 0)
            py_decoded = base64.a85decode(proc2.stdout, foldspaces=False, adobe=False)
            self.assertEqual(py_decoded, data)

    def test_decode_invalid_data_nonzero_exit(self):
        """Некорректные данные → декодер должен завершаться с ненулевым кодом"""
        if not os.path.exists(self.ascii85_exe):
            self.skipTest(f"Исполняемый файл не найден: {self.ascii85_exe}")

        cases = [
            b"5~",          
            b"<~5sdq,~>",   
        ]
        for invalid in cases:
            proc = subprocess.run([self.ascii85_exe, '-db'], input=invalid, capture_output=True, text=False)
            self.assertNotEqual(proc.returncode, 0, f"Ожидается ненулевой код возврата для {invalid!r}")

if __name__ == '__main__':
    unittest.main()
