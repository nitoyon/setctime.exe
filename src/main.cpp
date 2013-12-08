/* The MIT License (MIT)
 *
 * Copyright (c) 2013 nitoyon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// We don't use C Run-Time library in order to
//   * reduce the file size
//   * make it possible to run the EXE file on any PC even if VC++ Runtime is
//     not installed

#include <windows.h>
#include <tchar.h>

int wmain()
{
	int ret = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argv == NULL || argc != 2)
	{
		ret = 1;
		goto done;
	}

	hFile = CreateFile(argv[1], FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, 0,
		NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		ret = 2;
		goto done;
	}

	FILETIME ft;
	if (!GetFileTime(hFile, NULL, NULL, &ft))
	{
		ret = 3;
		goto done;
	}
	if (!SetFileTime(hFile, &ft, NULL, NULL))
	{
		ret = 4;
		goto done;
	}

done:
	if (argv != NULL)
	{
		LocalFree(argv);
	}
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}

	return ret;
}
