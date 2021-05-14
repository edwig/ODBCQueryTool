////////////////////////////////////////////////////////////////////////
//
// File: DIB.h
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
#pragma once

class StyleDIB : public CObject
{
public:
    StyleDIB();
    virtual ~StyleDIB();

public:
    int     GetNumClrEntries();                     // Number of color table entries
    BOOL    Create(int width, int height);          // Create a new DIB
    BOOL    Create(BITMAPINFO* pBMI, BYTE* pBits);  // Create from existing memory buffer
    void*   GetPixelAddress(int x, int y);
    virtual BOOL Load(CFile* fp);                   // Load from file
    virtual BOOL Load(char* pszFileName = NULL);    // Load DIB from disk file
    virtual BOOL LoadBitmap(WORD wResid);
    virtual BOOL Load(WORD wResid);                 // Load DIB from resource
    virtual BOOL Save(char* pszFileName = NULL);    // Save DIB to disk file
    virtual BOOL Save(CFile* fp);                   // Save to file
    virtual void Draw(CDC* pDC, int x, int y);
    virtual void Draw(CDC* pDC, int x, int y, int w, int h);
    virtual void Draw(CDC* pDC, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
    virtual int  GetWidth();                       // Image width
    virtual int  GetHeight();                      // Image height
    virtual BOOL MapColorsToPalette(CPalette* pPal);
    virtual void GetRect(CRect* pRect);
    virtual void CopyBits(StyleDIB* pDIB, 
                          int xd, int yd,
                          int w,  int h,
                          int xs, int ys,
                          COLORREF clrTrans = 0xFFFFFFFF);
protected:
    BITMAPINFO* m_pBMI;         // Pointer to BITMAPINFO struct
    BYTE* m_pBits;              // Pointer to the bits
    BOOL  m_bMyBits;            // TRUE if DIB owns Bits memory

private:
    int DibWidth();             // Width  in pixels
    int DibHeight();            // Height in pixels
    int StorageWidth();         // Storage width in bytes
    int DibPlanes();            // Bitmap color planes
    int DibBitCount();          // Bitmap bit count
    BITMAPINFOHEADER* GetBitmapHeaderAdress()  {return &(m_pBMI->bmiHeader); }
    BITMAPINFO*       GetBitmapInfoAddress()   {return m_pBMI;}     // Pointer to bitmap info
    void*             GetBitsAddress()         {return m_pBits;}    // Pointer to the bits
    RGBQUAD*          GetClrTabAddress()
    {
      // Pointer to color table
      return reinterpret_cast<LPRGBQUAD>( reinterpret_cast<BYTE*>(m_pBMI) + sizeof(BITMAPINFOHEADER));
    }
};

