void DisplayBitmap (uint32_t xoff, uint32_t yoff, char* bmpName) {
	// Open the file
	HANDLE fHandle = sdCreateFile(bmpName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (fHandle != 0) {
		uint32_t bytesRead;
		BITMAPFILEHEADER bmpHeader;
		if ((sdReadFile(fHandle, &bmpHeader, sizeof(bmpHeader), &bytesRead, 0) == true) &&
			(bytesRead == sizeof(bmpHeader)) &&
			(bmpHeader.bfType == 19778)) {
			BITMAPINFOHEADER bmpInfo;

			if ((sdReadFile(fHandle, &bmpInfo, sizeof(bmpInfo), &bytesRead, 0) == true) &&
				(bytesRead == sizeof(bmpInfo))){
				uint32_t xferWidth, bytesPerPixel;
				uint8_t buf[4096];
				if (bmpInfo.biBitCount == 24) bytesPerPixel = 3;
					else bytesPerPixel = 4;
				xferWidth = bmpInfo.biWidth * bytesPerPixel;
				int p2Mod = xferWidth % 4;
				if (p2Mod != 0) xferWidth += (4 - p2Mod);
				sdSetFilePointer(fHandle, bmpHeader.bfOffBits, 0, FILE_BEGIN);		// Set file pointer to bitmap data
				for (int y = bmpInfo.biHeight; y > 0; y--) {
					sdReadFile(fHandle, &buf[0], xferWidth, &bytesRead, 0);
					CvtBmpLine(GetConsoleDC(), xoff, yoff + y, bmpInfo.biWidth, bmpInfo.biBitCount, &buf[0]);
				}
			}
			else printf("Bitmap file: %s, is not a valid format\n", bmpName);
		}
		// Close the file
		sdCloseHandle(fHandle);
	}
}

void DisplayDirectory(const char* dirName) {
	HANDLE fh;
	FIND_DATA find;
	char* month[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	fh = sdFindFirstFile(dirName, &find);							// Find first file
	do {
		if (find.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			printf("%s <DIR>\n", find.cFileName);
		else printf("%c%c%c%c%c%c%c%c.%c%c%c Size: %9lu bytes, %2d/%s/%4d, LFN: %s\n",
			find.cAlternateFileName[0], find.cAlternateFileName[1],
			find.cAlternateFileName[2], find.cAlternateFileName[3],
			find.cAlternateFileName[4], find.cAlternateFileName[5],
			find.cAlternateFileName[6], find.cAlternateFileName[7],
			find.cAlternateFileName[8], find.cAlternateFileName[9],
			find.cAlternateFileName[10],
			(unsigned long)find.nFileSizeLow,
			find.CreateDT.tm_mday, month[find.CreateDT.tm_mon],
			find.CreateDT.tm_year + 1900,
			find.cFileName);										// Display each entry
	} while (sdFindNextFile(fh, &find) != 0);						// Loop finding next file
	sdFindClose(fh);												// Close the serach handle
}
