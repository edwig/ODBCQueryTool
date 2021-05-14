////////////////////////////////////////////////////////////////////////
//
// File: StyleDIB.cpp
// Function: Device Independent Bitmap
//
//   _____ _         _ _             ______                                           _    
//  / ____| |       | (_)           |  ____|                                         | |   
// | (___ | |_ _   _| |_ _ __   __ _| |__ _ __ __ _ _ __ ___   _____      _____  _ __| | __
//  \___ \| __| | | | | | '_ \ / _` |  __| '__/ _` | '_ ` _ \ / _ \ \ /\ / / _ \| '__| |/ /
//  ____) | |_| |_| | | | | | | (_| | |  | | | (_| | | | | | |  __/\ V  V / (_) | |  |   < 
// |_____/ \__|\__, |_|_|_| |_|\__, |_|  |_|  \__,_|_| |_| |_|\___| \_/\_/ \___/|_|  |_|\_\
//              __/ |           __/ |                                                      
//             |___/           |___/                                                       
//
//
// Author: ir. W.E. Huisman
// For license: See the file "LICENSE.txt" in the root folder
//
#include "stdafx.h"
#include "StyleDIB.h"
#include "StyleMessageBox.h"
#include "StyleTexts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Create a small DIB here so m_pBMI and m_pBits are always valid.
StyleDIB::StyleDIB()
{
  m_pBMI    = NULL;
  m_pBits   = NULL;
  m_bMyBits = TRUE;
}

StyleDIB::~StyleDIB()
{
  // Free the memory.
  if (m_pBMI != NULL)
  {
    free(m_pBMI);
  }
  if (m_bMyBits && (m_pBits != NULL))
  {
    free(m_pBits);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Private functions

static BOOL IsWinDIB(BITMAPINFOHEADER* pBIH)
{
  if (((BITMAPCOREHEADER*)pBIH)->bcSize == sizeof(BITMAPCOREHEADER))
  {
    return FALSE;
  }
  return TRUE;
}

static int 
NumDIBColorEntries(BITMAPINFO* pBmpInfo)
{
  BITMAPINFOHEADER* pBIH;
  BITMAPCOREHEADER* pBCH;
  int iColors, iBitCount;

  pBIH = &(pBmpInfo->bmiHeader);
  pBCH = (BITMAPCOREHEADER*)pBIH;

  // Start off by assuming the color table size from
  // the bit-per-pixel field.
  if (IsWinDIB(pBIH))
  {
    iBitCount = pBIH->biBitCount;
  }
  else
  {
    iBitCount = pBCH->bcBitCount;
  }

  switch (iBitCount)
  {
    case 1:  iColors = 2;        break;
    case 4:  iColors = 16;       break;
    case 8:  iColors = 256;      break;
    default: iColors = 0;        break;
  }

  // If this is a Windows DIB, then the color table length
  // is determined by the biClrUsed field if the value in
  // the field is nonzero.
  if (IsWinDIB(pBIH) && (pBIH->biClrUsed != 0))
  {
    iColors = pBIH->biClrUsed;
  }
  // BUGFIX 18 Oct 94 NigelT
  // Make sure the value is reasonable since some products
  // will write out more then 256 colors for an 8 bpp DIB!!!
  int iMax = 0;
  switch (iBitCount)
  {
    case 1:   iMax = 2;        break;
    case 4:   iMax = 16;       break;
    case 8:   iMax = 256;      break;
    default:  iMax = 0;        break;
  }
  if (iMax)
  {
    if (iColors > iMax)
    {
      TRACE("Invalid color count");
      iColors = iMax;
    }
  }
  return iColors;
}


/////////////////////////////////////////////////////////////////////////////
// StyleDIB commands

// Create a new empty 8bpp DIB with a 256 entry color table.
BOOL StyleDIB::Create(int iWidth, int iHeight)
{
  // Delete any existing stuff.
  if (m_pBMI != NULL) ::free(m_pBMI);
  if (m_bMyBits && (m_pBits != NULL)) ::free(m_pBits);

  // Allocate memory for the header.
  m_pBMI = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)
    + 256 * sizeof(RGBQUAD));
  if (!m_pBMI)
  {
    TRACE("Out of memory for DIB header");
    return FALSE;
  }
  // Allocate memory for the bits (DWORD aligned).
  int iBitsSize = ((iWidth + 3) & ~3) * iHeight;
  m_pBits = (BYTE*)malloc(iBitsSize);
  if (!m_pBits)
  {
    TRACE("Out of memory for DIB bits");
    ::free(m_pBMI);
    m_pBMI = NULL;
    return FALSE;
  }
  m_bMyBits = TRUE;

  // Fill in the header info.
  BITMAPINFOHEADER* pBI = (BITMAPINFOHEADER*)m_pBMI;
  pBI->biSize           = sizeof(BITMAPINFOHEADER);
  pBI->biWidth          = iWidth;
  pBI->biHeight         = iHeight;
  pBI->biPlanes         = 1;
  pBI->biBitCount       = 8;
  pBI->biCompression    = BI_RGB;
  pBI->biSizeImage      = 0;
  pBI->biXPelsPerMeter  = 0;
  pBI->biYPelsPerMeter  = 0;
  pBI->biClrUsed        = 0;
  pBI->biClrImportant   = 0;

  // Create an arbitrary color table (gray scale).
  RGBQUAD* prgb = GetClrTabAddress();
  for (int i = 0; i < 256; i++)
  {
    prgb->rgbBlue = prgb->rgbGreen = prgb->rgbRed = (BYTE)i;
    prgb->rgbReserved = 0;
    prgb++;
  }
  // Set all the bits to a known state (black).
  memset(m_pBits, 0, iBitsSize);
  return TRUE;
}

// Create a StyleDIB structure from existing header and bits. The DIB
// won't delete the bits and makes a copy of the header.
BOOL StyleDIB::Create(BITMAPINFO* pBMI, BYTE* pBits)
{
  if (m_pBMI != NULL) ::free(m_pBMI);
  m_pBMI = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
  // Note: This will probably fail for < 256 color headers.
  memcpy(m_pBMI, pBMI, sizeof(BITMAPINFOHEADER) +  NumDIBColorEntries(pBMI) * sizeof(RGBQUAD));

  if (m_bMyBits && (m_pBits != NULL))
  {
    ::free(m_pBits);
  }
  m_pBits   = pBits;
  m_bMyBits = FALSE; // We can't delete the bits.
  return TRUE;
}

// Load a DIB from an open file.
BOOL StyleDIB::Load(CFile* fp)
{
  BOOL bIsPM = FALSE;
  BITMAPINFO* pBmpInfo = NULL;
  BYTE* pBits = NULL;

  // Get the current file position.
  ULONGLONG dwFileStart = fp->GetPosition();

  // Read the file header to get the file size and to
  // find where the bits start in the file.
  BITMAPFILEHEADER BmpFileHdr;
  int iBytes;
  iBytes = fp->Read(&BmpFileHdr, sizeof(BmpFileHdr));
  if (iBytes != sizeof(BmpFileHdr))
  {
    TRACE("Failed to read file header");
    goto $abort;
  }
  // Check that we have the magic 'BM' at the start.
  if (BmpFileHdr.bfType != 0x4D42)
  {
    TRACE("Not a bitmap file");
    goto $abort;
  }
  // Make a wild guess that the file is in Windows DIB
  // format and read the BITMAPINFOHEADER. If the file turns
  // out to be a PM DIB file we'll convert it later.
  BITMAPINFOHEADER BmpInfoHdr;
  iBytes = fp->Read(&BmpInfoHdr, sizeof(BmpInfoHdr));
  if (iBytes != sizeof(BmpInfoHdr))
  {
    TRACE("Failed to read BITMAPINFOHEADER");
    goto $abort;
  }

  // Check that we got a real Windows DIB file.
  if (BmpInfoHdr.biSize != sizeof(BITMAPINFOHEADER))
  {
    if (BmpInfoHdr.biSize != sizeof(BITMAPCOREHEADER))
    {
      TRACE(" File is not Windows or PM DIB format");
      goto $abort;
    }
    // Set a flag to convert PM file to Win format later.
    bIsPM = TRUE;

    // Back up the file pointer and read the BITMAPCOREHEADER
    // and create the BITMAPINFOHEADER from it.
    fp->Seek((LONG)(dwFileStart + sizeof(BITMAPFILEHEADER)), CFile::begin);
    BITMAPCOREHEADER BmpCoreHdr;
    iBytes = fp->Read(&BmpCoreHdr, sizeof(BmpCoreHdr));
    if (iBytes != sizeof(BmpCoreHdr))
    {
      TRACE("Failed to read BITMAPCOREHEADER");
      goto $abort;
    }
    BmpInfoHdr.biSize           = sizeof(BITMAPINFOHEADER);
    BmpInfoHdr.biWidth          = (int)BmpCoreHdr.bcWidth;
    BmpInfoHdr.biHeight         = (int)BmpCoreHdr.bcHeight;
    BmpInfoHdr.biPlanes         = BmpCoreHdr.bcPlanes;
    BmpInfoHdr.biBitCount       = BmpCoreHdr.bcBitCount;
    BmpInfoHdr.biCompression    = BI_RGB;
    BmpInfoHdr.biSizeImage      = 0;
    BmpInfoHdr.biXPelsPerMeter  = 0;
    BmpInfoHdr.biYPelsPerMeter  = 0;
    BmpInfoHdr.biClrUsed        = 0;
    BmpInfoHdr.biClrImportant   = 0;
  }
  // Work out how much memory we need for the BITMAPINFO
  // structure, color table and then for the bits.  
  // Allocate the memory blocks.
  // Copy the BmpInfoHdr we have so far,
  // and then read in the color table from the file.
  int iColors;
  int iColorTableSize;
  iColors = NumDIBColorEntries((LPBITMAPINFO)&BmpInfoHdr);
  iColorTableSize = iColors * sizeof(RGBQUAD);
  int iBitsSize;
  int iBISize;
  // Always allocate enough room for 256 entries.
  iBISize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
  iBitsSize = BmpFileHdr.bfSize - BmpFileHdr.bfOffBits;

  // Allocate the memory for the header.
  pBmpInfo = (LPBITMAPINFO)malloc(iBISize);
  if (!pBmpInfo)
  {
    TRACE("Out of memory for DIB header");
    goto $abort;
  }

  // Copy the header we already have.
  memcpy(pBmpInfo, &BmpInfoHdr, sizeof(BITMAPINFOHEADER));

  // Now read the color table from the file.
  if (bIsPM == FALSE) 
  {
    // Read the color table from the file.
    iBytes = fp->Read(((LPBYTE)pBmpInfo) + sizeof(BITMAPINFOHEADER),iColorTableSize);
    if (iBytes != iColorTableSize)
    {
      TRACE("Failed to read color table");
      goto $abort;
    }
  }
  else
  {
    // Read each PM color table entry in turn and convert it
    // to Win DIB format as we go.
    LPRGBQUAD lpRGB;
    lpRGB = (LPRGBQUAD)((LPBYTE)pBmpInfo + sizeof(BITMAPINFOHEADER));
    int i;
    RGBTRIPLE rgbt;
    for (i = 0; i < iColors; i++)
    {
      iBytes = fp->Read(&rgbt, sizeof(RGBTRIPLE));
      if (iBytes != sizeof(RGBTRIPLE))
      {
        TRACE("Failed to read RGBTRIPLE");
        goto $abort;
      }
      lpRGB->rgbBlue     = rgbt.rgbtBlue;
      lpRGB->rgbGreen    = rgbt.rgbtGreen;
      lpRGB->rgbRed      = rgbt.rgbtRed;
      lpRGB->rgbReserved = 0;
      lpRGB++;
    }
  }
  // Allocate the memory for the bits
  // and read the bits from the file.
  pBits = (BYTE*)malloc(iBitsSize);
  if (!pBits)
  {
    TRACE("Out of memory for DIB bits");
    goto $abort;
  }
  // Seek to the bits in the file.
  fp->Seek((LONG)(dwFileStart + BmpFileHdr.bfOffBits), CFile::begin);

  // Read the bits.
  iBytes = fp->Read(pBits, iBitsSize);
  if (iBytes != iBitsSize)
  {
    TRACE("Failed to read bits");
    goto $abort;
  }
  // Everything went OK.
  if (m_pBMI != NULL)
  {
    ::free(m_pBMI);
  }
  m_pBMI = pBmpInfo;
  if (m_bMyBits && (m_pBits != NULL))
  {
    ::free(m_pBits);
  }
  m_pBits = pBits;
  m_bMyBits = TRUE;
  return TRUE;

$abort: // Something went wrong.
  if (pBmpInfo)
  {
    free(pBmpInfo);
  }
  if (pBits)
  {
    free(pBits);
  }
  return FALSE;
}

// Load a DIB from a disk file. If no file name is given, show
// an Open File dialog to get one.
BOOL StyleDIB::Load(char* pszFileName)
{
  // Try to open the file for read access.
  CFile file;
  if (!file.Open(pszFileName, CFile::modeRead | CFile::shareDenyWrite))
  {
    TRACE("Failed to open file");
    return FALSE;
  }
  BOOL bResult = Load(&file);
  file.Close();
  return bResult;
}

// Load a DIB from a DIB resource id.
BOOL StyleDIB::LoadBitmap(WORD wResid)
{
  HINSTANCE hInst = AfxGetResourceHandle();
  HRSRC hrsrc = ::FindResource(hInst, MAKEINTRESOURCE(wResid), RT_BITMAP);
  if (!hrsrc)
  {
    TRACE("DIB resource not found");
    return FALSE;
  }

  HGLOBAL hg = LoadResource(hInst, hrsrc);
  if (!hg)
  {
    TRACE("Failed to load DIB resource");
    return FALSE;
  }
  BYTE* pRes = (BYTE*)LockResource(hg);
  int iSize = ::SizeofResource(hInst, hrsrc);

  // Mark the resource pages as read/write so the mmioOpen
  // won't fail
  DWORD dwOldProt;
  /* BOOL b = */::VirtualProtect(pRes,iSize,PAGE_READWRITE,&dwOldProt);
  //BITMAPINFO* bm = (BITMAPINFO*)pRes;
  CSize size;

  return FALSE;
}

// Load a DIB from a DIB resource id.
BOOL StyleDIB::Load(WORD wResid)
{
  HINSTANCE hInst = AfxGetResourceHandle();
  HRSRC hrsrc = ::FindResource(hInst, MAKEINTRESOURCE(wResid), "DIB");
  if (!hrsrc)
  {
    TRACE("DIB resource not found");
    return FALSE;
  }
  HGLOBAL hg = LoadResource(hInst, hrsrc);
  if (!hg)
  {
    TRACE("Failed to load DIB resource");
    return FALSE;
  }
  BYTE* pRes = (BYTE*)LockResource(hg);
  int iSize = ::SizeofResource(hInst, hrsrc);

  // Mark the resource pages as read/write so the mmioOpen
  // won't fail
  DWORD dwOldProt;
  ::VirtualProtect(pRes,iSize,PAGE_READWRITE,&dwOldProt);
  // Now create the StyleDIB object. We will create a new header from the 
  // data in the resource image and copy the bits from the resource
  // to a new block of memory.  We can't use the resource image as-is 
  // because we might want to map the DIB colors and the resource memory
  // is write protected in Win32.

  BITMAPFILEHEADER* pFileHdr = (BITMAPFILEHEADER*)pRes;
  if (pFileHdr->bfType != 0x4D42)
  {
    CString error   = GetStyleText(TXT_ERROR);
    CString message = GetStyleText(TXT_NO_BITMAP);
    StyleMessageBox(nullptr,message,error,MB_OK | MB_ICONERROR);
    return FALSE;
  }
  BITMAPINFOHEADER* pInfoHdr = (BITMAPINFOHEADER*)(pRes + sizeof(BITMAPFILEHEADER));
  if (pInfoHdr->biSize != sizeof(BITMAPINFOHEADER))
  {
    CString error   = GetStyleText(TXT_ERROR);
    CString message = GetStyleText(TXT_NO_BITMAP);
    StyleMessageBox(nullptr,message,error,MB_OK | MB_ICONERROR);
    return FALSE;
  }
  BYTE* pBits = pRes + pFileHdr->bfOffBits;
  return Create((BITMAPINFO*)pInfoHdr, pBits);
  // Note: not required to unlock or free the resource in Win32
}

// Draw the DIB to a given DC.
void StyleDIB::Draw(CDC* pDC, int x, int y)
{
  ::StretchDIBits(pDC->GetSafeHdc(),
    x,                        // Destination x
    y,                        // Destination y
    DibWidth(),               // Destination width
    DibHeight(),              // Destination height
    0,                        // Source x
    0,                        // Source y
    DibWidth(),               // Source width
    DibHeight(),              // Source height
    GetBitsAddress(),         // Pointer to bits
    GetBitmapInfoAddress(),   // BITMAPINFO
    DIB_RGB_COLORS,           // Options
    SRCCOPY);                 // Raster operation code (ROP)
}

void StyleDIB::Draw(CDC* pDC, int x, int y, int w, int h)
{
  ::StretchDIBits(pDC->GetSafeHdc(),
                  x,                        // Destination x
                  y,                        // Destination y
                  w,                        // Destination width
                  h,                        // Destination height
                  0,                        // Source x
                  0,                        // Source y
                  DibWidth(),               // Source width
                  DibHeight(),              // Source height
                  GetBitsAddress(),         // Pointer to bits
                  GetBitmapInfoAddress(),   // BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  SRCCOPY);                 // Raster operation code (ROP)
}

void StyleDIB::Draw(CDC* pDC, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
  ::StretchDIBits(pDC->GetSafeHdc(),
                  x,                        // Destination x
                  y,                        // Destination y
                  w,                        // Destination width
                  h,                        // Destination height
                  sx,                       // Source x
                  sy,                       // Source y
                  sw,                       // Source width
                  sh,                       // Source height
                  GetBitsAddress(),         // Pointer to bits
                  GetBitmapInfoAddress(),   // BITMAPINFO
                  DIB_RGB_COLORS,           // Options
                  SRCCOPY);                 // Raster operation code (ROP)
}


// Get the number of color table entries.
int StyleDIB::GetNumClrEntries()
{
  return NumDIBColorEntries(m_pBMI);
}

// NOTE: This assumes all StyleDIB objects have 256 color table entries.
BOOL StyleDIB::MapColorsToPalette(CPalette* pPal)
{
  if (!pPal)
  {
    TRACE("No palette to map to");
    return FALSE;
  }
  LPRGBQUAD pctThis = GetClrTabAddress();
  // Build an index translation table to map this DIBs colors
  // to those of the reference DIB.
  BYTE imap[256];
  int iChanged = 0; // For debugging only
  for (int i = 0; i < 256; i++)
  {
    imap[i] = (BYTE)pPal->GetNearestPaletteIndex(RGB(pctThis->rgbRed,pctThis->rgbGreen,pctThis->rgbBlue));
    pctThis++;
    if (imap[i] != i)
    {
      iChanged++; // For debugging
    }
  }
  // Now map the DIB bits.
  BYTE* pBits = (BYTE*)GetBitsAddress();
  int iSize = StorageWidth() * DibHeight();
  while (iSize--)
  {
    *pBits = imap[*pBits];
    pBits++;
  }
  // Now reset the DIB color table so that its RGB values match
  // those in the palette.
  PALETTEENTRY pe[256];
  pPal->GetPaletteEntries(0, 256, pe);
  pctThis = GetClrTabAddress();
  for (int i = 0; i < 256; i++)
  {
    pctThis->rgbRed = pe[i].peRed;
    pctThis->rgbGreen = pe[i].peGreen;
    pctThis->rgbBlue = pe[i].peBlue;
    pctThis++;
  }
  // Now say all the colors are in use
  m_pBMI->bmiHeader.biClrUsed = 256;
  return TRUE;
}

// Get a pointer to a pixel.
// NOTE: DIB scan lines are DWORD aligned. The scan line 
// storage width may be wider than the scan line image width
// so calc the storage width by rounding the image width 
// to the next highest DWORD value.
void* StyleDIB::GetPixelAddress(int x, int y)
{
  int iWidth;
  // Make sure it's in range and if it isn't return zero.
  if ((x >= DibWidth()) || (y >= DibHeight()))
  {
    TRACE("Attempt to get out of range pixel address");
    return NULL;
  }
  // Calculate the scan line storage width.
  iWidth = StorageWidth();
  return m_pBits + (DibHeight() - y - 1) * iWidth + x;
}

// Get the bounding rectangle.
void StyleDIB::GetRect(CRect* pRect)
{
  pRect->top = 0;
  pRect->left = 0;
  pRect->bottom = DibHeight();
  pRect->right = DibWidth();
}

// Copy a rectangle of the DIB to another DIB.
// Note: We only support 8bpp DIBs here.
void StyleDIB::CopyBits(StyleDIB* pdibDest,
                    int xd, int yd,
                    int w,  int h,
                    int xs, int ys,
                    COLORREF clrTrans)
{
  // Test for silly cases.
  if (w == 0 || h == 0) return;

  // Get pointers to the start points in the source and destination
  // DIBs. Note that the start points will be the bottom-left
  // corner of the DIBs because the scan lines are reversed in memory.
  BYTE* pSrc = (BYTE*)GetPixelAddress(xs, ys + h - 1);
  BYTE* pDest = (BYTE*)pdibDest->GetPixelAddress(xd, yd + h - 1);

  // Get the scan line widths of each DIB.
  int iScanS = StorageWidth();
  int iScanD = pdibDest->StorageWidth();

  if (clrTrans == 0xFFFFFFFF)
  {
    // Copy the lines.
    while (h--)
    {
      memcpy(pDest, pSrc, w);
      pSrc += iScanS;
      pDest += iScanD;
    }
  }
  else
  {
    // Copy lines with transparency.
    // Note: We accept only a PALETTEINDEX description
    // for the color definition.
    // ASSERT((clrTrans & 0xFF000000) == 0x01000000);
    BYTE bTransClr = LOBYTE(LOWORD(clrTrans));
    int iSinc = iScanS - w; // Source increment value
    int iDinc = iScanD - w; // Destination increment value
    int iCount;
    BYTE pixel;
    while (h--)
    {
      iCount = w;    // Number of pixels to scan.
      while (iCount--)
      {
        pixel = *pSrc++;
        // Copy pixel only if it isn't transparent.
        if (pixel != bTransClr)
        {
          *pDest++ = pixel;
        }
        else
        {
          pDest++;
        }
      }
      // Move on to the next line.
      pSrc += iSinc;
      pDest += iDinc;
    }
  }
}

// Save a DIB to a disk file.
// This is somewhat simplistic because we only deal with 256 color DIBs
// and we always write a 256 color table.
BOOL StyleDIB::Save(CFile* fp)
{
  BITMAPFILEHEADER bfh;

  // Construct the file header.
  bfh.bfType = 0x4D42; // 'BM'
  bfh.bfSize = sizeof(BITMAPFILEHEADER) +
               sizeof(BITMAPINFOHEADER) +
               256 * sizeof(RGBQUAD)    +
               StorageWidth() * DibHeight();
  bfh.bfReserved1 = 0;
  bfh.bfReserved2 = 0;
  bfh.bfOffBits = sizeof(BITMAPFILEHEADER) +
                  sizeof(BITMAPINFOHEADER) +
                  256 * sizeof(RGBQUAD);

  // Write the file header.
  int iSize = sizeof(bfh);
  TRY
  {
      fp->Write(&bfh, iSize);
  }
    CATCH(CFileException, e)
  {
    TRACE("Failed to write file header");
    return FALSE;
  }
  END_CATCH;

  // Write the BITMAPINFO structure.
  // Note: we assume that there are always 256 colors in the
  // color table.
  iSize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
  TRY
  {
      fp->Write(m_pBMI, iSize);
  }
  CATCH(CFileException, e)
  {
    TRACE("Failed to write BITMAPINFO");
    return FALSE;
  }
  END_CATCH;

  // Write the bits.
  iSize = StorageWidth() * DibHeight();
  TRY
  {
      fp->Write(m_pBits, iSize);
  }
  CATCH(CFileException, e)
  {
    TRACE("Failed to write bits");
    return FALSE;
  }
  END_CATCH;

  return TRUE;
}

// Save a DIB to a disk file. If no file name is given, show
// a File Save dialog to get one.
BOOL StyleDIB::Save(char* pszFileName)
{
  // Try to open the file for write access.
  CFile file;
  if (!file.Open(pszFileName, CFile::modeReadWrite | CFile::modeCreate | CFile::shareExclusive))
  {
    // Cannot open the file: %s
    CString error;
    error.Format(GetStyleText(TXT_FILE_NOT_OPEN),pszFileName);
    StyleMessageBox(nullptr,error,GetStyleText(TXT_ERROR),MB_OK | MB_ICONERROR);
    return FALSE;
  }

  BOOL bResult = Save(&file);
  file.Close();

  if (!bResult)
  {
    // Cannot close the file: %s
    CString error;
    error.Format(GetStyleText(TXT_FILE_NOT_CLOSED),pszFileName);
    StyleMessageBox(nullptr,error,GetStyleText(TXT_ERROR),MB_OK | MB_ICONERROR);
  }
  return bResult;
}

int
StyleDIB::GetWidth()
{
  return DibWidth();
}

int
StyleDIB::GetHeight()
{
  return DibHeight();
}

int
StyleDIB::DibWidth()
{
  if (m_pBMI)
  {
    return m_pBMI->bmiHeader.biWidth;
  }
  return 0;
}

int
StyleDIB::DibHeight()
{
  if (m_pBMI)
  {
    return m_pBMI->bmiHeader.biHeight;
  }
  return 0;
}

int
StyleDIB::StorageWidth()
{
  if (m_pBMI)
  {
    return (m_pBMI->bmiHeader.biWidth + 3) & ~3;
  }
  return 0;
}

int
StyleDIB::DibPlanes()
{
  if (m_pBMI)
  {
    return (m_pBMI->bmiHeader.biPlanes);
  }
  return 0;
}

int
StyleDIB::DibBitCount()
{
  if (m_pBMI)
  {
    return (m_pBMI->bmiHeader.biBitCount);
  }
  return 0;
}
