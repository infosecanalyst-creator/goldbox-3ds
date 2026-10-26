// Classes/Display.cpp - hand port of Classes/Display.cs.
// coab drew into a 24bpp System.Drawing.Bitmap; we keep the same EGA index
// buffer (`ram`) and BGR video RAM, and hand frames to platform::PresentFrame.
// Copyright (c) 2026, the goldbox-3ds contributors. BSD 3-clause (see LICENSE).
#include "Classes/Classes.h"
#include "platform/platform.h"

namespace Classes {

bool Display::StaticInit() {
    outputHeight = 200;
    outputWidth = 320;

    ram = Array2<int>(outputHeight, outputWidth);
    scanLineWidth = outputWidth * 3;
    videoRamSize = scanLineWidth * outputHeight;
    videoRam = Array<byte>(videoRamSize);

    // bm = new Bitmap(outputWidth, outputHeight, PixelFormat.Format24bppRgb);  -- dropped
    return true;
}

void Display::DisplayMono8x8(int xCol, int yCol, Array<byte> monoData8x8, int bgColor, int fgColor) {
    int pX = xCol * 8;

    for (int yStep = 0; yStep < 8; yStep++) {
        int pY = (yCol * 8) + yStep;
        int value = gbl::monoCharData[yStep];  // PORT-NOTE: coab reads gbl.monoCharData, not the parameter

        for (int i = 0; i < 8; i++) {
            ram(pY, pX + i) = (value & MonoBitMask[i]) != 0 ? fgColor : bgColor;
            SetVidPixel(pX + i, pY, ram(pY, pX + i));
        }
    }
}

void Display::SetEgaPalette(int index, int colour) {
    egaColors(index, 0) = OrigEgaColors(colour, 0);
    egaColors(index, 1) = OrigEgaColors(colour, 1);
    egaColors(index, 2) = OrigEgaColors(colour, 2);

    for (int y = 0; y < outputHeight; y++) {
        int vy = y * scanLineWidth;
        for (int x = 0; x < outputWidth; x++) {
            int vx = x * 3;
            int egaColor = ram(y, x);

            videoRam[vy + vx + 0] = egaColors(egaColor, 2);
            videoRam[vy + vx + 1] = egaColors(egaColor, 1);
            videoRam[vy + vx + 2] = egaColors(egaColor, 0);
        }
    }

    Display::Update();
}

void Display::SetVidPixel(int x, int y, int egaColor) {
    videoRam[(y * scanLineWidth) + (x * 3) + 0] = egaColors(egaColor, 2);
    videoRam[(y * scanLineWidth) + (x * 3) + 1] = egaColors(egaColor, 1);
    videoRam[(y * scanLineWidth) + (x * 3) + 2] = egaColors(egaColor, 0);
}

void Display::UpdateStop() { noUpdateCount++; }

void Display::UpdateStart() {
    noUpdateCount--;
    Update();
}

void Display::Update() {
    if (noUpdateCount == 0) {
        RawCopy(videoRam, videoRamSize);

        if (updateCallback != nullptr) {
            updateCallback();
        }
    }
}

void Display::ForceUpdate() {
    RawCopy(videoRam, videoRamSize);

    if (updateCallback != nullptr) {
        updateCallback();
    }
}

void Display::SaveVidRam() { videoRamBkUp = videoRam.Clone(); }

void Display::RestoreVidRam() { videoRam = videoRamBkUp; }

byte Display::GetPixel(int x, int y) { return (byte)ram(y, x); }

void Display::SetPixel3(int x, int y, int value) {
    if (value < 16) {
        ram(y, x) = value;

        SetVidPixel(x, y, ram(y, x));
    }
    if (value > 16) {
    }
}

// coab copied videoRam (BGR24) into a Bitmap. We present from the EGA index
// buffer instead, which is what the 3DS renderer wants. NOTE: after
// RestoreVidRam() coab's videoRam and ram can disagree until the next redraw;
// we present the BGR videoRam converted back to an index frame to stay
// visually identical to coab.
void Display::RawCopy(Array<byte> vram, int /*videoRamSize*/) {
    static int frame[320 * 200];
    uint8_t pal[16 * 3];
    for (int c = 0; c < 16; c++) {
        pal[c * 3 + 0] = egaColors(c, 0);
        pal[c * 3 + 1] = egaColors(c, 1);
        pal[c * 3 + 2] = egaColors(c, 2);
    }
    for (int y = 0; y < 200; y++) {
        for (int x = 0; x < 320; x++) {
            int idx = ram(y, x);
            int o = y * scanLineWidth + x * 3;
            // Fast path: index buffer agrees with video RAM.
            if (vram[o + 2] == pal[idx * 3 + 0] && vram[o + 1] == pal[idx * 3 + 1] && vram[o + 0] == pal[idx * 3 + 2]) {
                frame[y * 320 + x] = idx;
                continue;
            }
            for (int c = 0; c < 16; c++) {
                if (vram[o + 2] == pal[c * 3 + 0] && vram[o + 1] == pal[c * 3 + 1] && vram[o + 0] == pal[c * 3 + 2]) {
                    idx = c;
                    break;
                }
            }
            frame[y * 320 + x] = idx;
        }
    }
    platform::PresentFrame(frame, pal);
}

}  // namespace Classes
