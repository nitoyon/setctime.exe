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

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int SetCtime(LPWSTR path);

int wmain()
{
	int ret = 0;

	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argv == NULL)
	{
		wprintf_s(L"Failed to parse\n");
		ret = 1;
		goto done;
	}

	if (argc == 1)
	{
		wprintf_s(L"File not specified\n");
		ret = 1;
		goto done;
	}

	for (int i = 1; i < argc; i++)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(argv[i], &fd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			wprintf_s(L"%s ... FAIL\n", argv[i]);
			break;
		}

		do
		{
			wprintf_s(L"%s ... ", fd.cFileName);
			ret = SetCtime(fd.cFileName);
			if (ret != 0)
			{
				wprintf_s(L"FAIL (%d)\n", ret);
				FindClose(hFind);
				goto done;
			}
			wprintf_s(L"done\n");
		} while (FindNextFile(hFind, &fd));
	}

done:
	if (argv != NULL)
	{
		LocalFree(argv);
	}
}

int SetCtime(LPWSTR path)
{
	int ret = 0;
	HANDLE hFile = INVALID_HANDLE_VALUE;

	hFile = CreateFile(path, FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, 0,
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
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
	}

	return ret;
}
