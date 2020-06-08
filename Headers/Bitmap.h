#ifndef BITMAP_H__
#define BITMAP_H__

#include "Vector.h"

namespace GDI {
	struct Color {
		byte r, g, b, a;

		Color(byte r = 0, byte g = 0, byte b = 0, byte a = 255) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

		Color(uint colorData) {
			byte * color = (byte*)&colorData;
			a = color[3];
			r = color[2];
			g = color[1];
			b = color[0];
		}

		uint ToUInt() {
			byte * color = new byte[4]{ b, g, r, a };
			uint data = *(uint*)color;
			delete color;
			return data;
		}

		inline bool Compare(Color& color) {
			return (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a);
		}
	};

	class Bitmap {
		uint * pixels;
		BITMAPINFO info;
		const char * filename;

		inline void Jump(FILE* fptr, uint offset) {
			for (uint i = 0; i < offset; i++)
				fgetc(fptr);
		}

		template <class T> T GetBytes(FILE* fptr) {
			char * buffer = new char[sizeof(T)];
			for (uint i = 0, m = sizeof(T); i < m; i++)
				buffer[i] = fgetc(fptr);
			T bytes = *(T*)buffer;
			delete[] buffer;
			return bytes;
		}

		void Load() {
			FILE* fptr = fopen(filename, "r");
			if (!fptr) {
				LastErrorInfo::SetError(BITMAP_FILENOTFOUND);
				return;
			}

			// signature
			ushort signature = GetBytes<ushort>(fptr);

			if (signature != 19778) {
				LastErrorInfo::SetError(BITMAP_UNSUPPORTEDFORMAT);
				return;
			}

			// file Size.
			uint fileSize = GetBytes<uint>(fptr);
			uint reserved = GetBytes<uint>(fptr);
			uint offsetPixelData = GetBytes<uint>(fptr);

			// filling of bitmap informaiton.
			this->info.bmiHeader = GetBytes<BITMAPINFOHEADER>(fptr);
			this->info.bmiHeader.biHeight = -this->info.bmiHeader.biHeight; //negative so that the image won't be flipped 
																			// after replacing bytes in the right order.
			// bit per pixel check:
			if (this->info.bmiHeader.biBitCount != 24) {
				LastErrorInfo::SetError(BITMAP_UNSUPPORTED_BITPERPIXEL);
				return;
			}

			// read pixel color.
			uint padding = (this->info.bmiHeader.biWidth * 3) % 4;

			// 24 bits integer-like to store the rgb codes.
			Int24 * rawData = (Int24*)malloc(sizeof(Int24) * this->info.bmiHeader.biWidth * -this->info.bmiHeader.biHeight);
			uint index = 0;

			for (uint i = 0, m = -this->info.bmiHeader.biHeight; i < m; i++) {
				for (uint j = 0; j < this->info.bmiHeader.biWidth; j++) {
					index = (i * this->info.bmiHeader.biWidth) + j;
					rawData[index] = GetBytes<Int24>(fptr);
				}

				// to jump over the padding 
				Jump(fptr, padding);
			}

			// conversion to 24 bits to 32 bits 
			this->info.bmiHeader.biBitCount = 32;

			pixels = (uint*)malloc(sizeof(uint) * this->info.bmiHeader.biWidth * -this->info.bmiHeader.biHeight);
			for (uint i = 0, m = this->info.bmiHeader.biWidth * -this->info.bmiHeader.biHeight, j = m; i < m; i++) {
				Color color = Color(rawData[j].high, rawData[j].medium, rawData[j].low, 255);
				this->pixels[i] = color.ToUInt();
			}

			//disposal of raw data.
			free(rawData);
		}

	public:
		Bitmap(const char * filename) {
			this->filename = filename;
			this->pixels = nullptr;
			this->info = MakeDummy<BITMAPINFO>();
			Load();
		}

		Bitmap(Color fillingColor, Point bmpSize) {
			//TODO: fill the bitmap with one color.
			uint size = bmpSize.x * bmpSize.y * 4;
			uint color = fillingColor.ToUInt();
			pixels = (uint*)malloc(sizeof(uint) * size);
			for (uint i = 0; i < size; i++)
				pixels[i] = color;

			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biHeight = -bmpSize.y;
			info.bmiHeader.biWidth = bmpSize.x;
			info.bmiHeader.biSizeImage = bmpSize.x * bmpSize.y;
			info.bmiHeader.biCompression = BI_RGB;
			info.bmiHeader.biClrUsed = 0;
			info.bmiHeader.biClrImportant = 0;
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biSize = sizeof(info.bmiHeader);
		}

		~Bitmap() {
			free(pixels);
		}

		inline Point Size() {
			return { this->info.bmiHeader.biWidth, -this->info.bmiHeader.biHeight };
		}

		inline uint Height() {
			return -this->info.bmiHeader.biHeight;
		}

		inline uint Width() {
			return -this->info.bmiHeader.biWidth;
		}

		inline BITMAPINFO Info() {
			return this->info;
		}

		inline const char * Filename() {
			return filename;
		}

		inline uint GetPixel(uint x, uint y) {
			return pixels[y * this->info.bmiHeader.biWidth + x];
		}

		inline void SetPixel(uint data, uint x, uint y) {
			pixels[y *  this->info.bmiHeader.biWidth + x] = data;
		}

		inline uint * Pixels() {
			return this->pixels;
		}

		inline uint& operator[](Point ptn) {
			return pixels[ptn.y * this->info.bmiHeader.biWidth + ptn.x];
		}

		void ApplyAlphaChannel() {
			// alpha channel color: 255, 255, 254. 
			// if the said color can be extracted, all said pixel will see their alpha value change.
		}

		void Fill(Color color) {
			for (uint i = 0, m = this->info.bmiHeader.biWidth * -this->info.bmiHeader.biHeight, icolor = color.ToUInt(); i < m; i++)
				pixels[i] = icolor;
		}

		void CopyBits(Bitmap& bmp, Point dest, Point src, Point srcSize, bool blend = false) {
			uint destIndex = 0;
			uint srcIndex = 0;

			for (uint desti = dest.y, m = desti + srcSize.y; desti < m; desti++) {
				for (uint destj = dest.x, n = destj + srcSize.x; destj < n; destj++) {
					// verify if the pixel is inside the window... You can't draw it if you can't see it! ;)
					if (destj && desti && (destj < this->info.bmiHeader.biWidth) && (desti < this->info.bmiHeader.biHeight)) {
						destIndex = desti * this->info.bmiHeader.biWidth + destj;
						srcIndex = (desti - src.y) * bmp.info.bmiHeader.biWidth + (destj - src.x);

						// if true, will blend the alpha values to make an effect of transparency.
						if (blend) {

						}
						else
							this->pixels[destIndex] = bmp.pixels[srcIndex];
					}
				}
			}
		}

		void StretchBits(Bitmap& bmp, Point dest, Point src, Point srcSize, Point srcStretch) {

		}

		void Draw() {

		}
	};
}

#endif
