////////////////////////////////////////////////////////////////////////
//
// File: SQLFormatErrors.h
//
// Copyright (c) 1998-2024 ir. W.E. Huisman
// All rights reserved
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies 
// or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version number: See SQLComponents.h
//
#pragma once

namespace SQLComponents
{

// Error values (< 0/OK) returned from SQLVariantFormat
// Error numbers are out of the range of error number range of other systems

#ifndef OK
#define OK                                                 0
#endif

#define ER_FormatDateTypeValue                         -1001
#define ER_FormatNumberNoNumber                        -1002
#define ER_FormatNumberTemplateDecimal                 -1003
#define ER_FormatNumberTemplateGetNumberFormat         -1004
#define ER_FormatNumberTemplateGetCurrencyFormat       -1005
#define ER_FormatNumberTemplateFormat                  -1006
#define ER_FormatNumberTemplateZero                    -1007
#define ER_FormatNumberTemplateNummer                  -1008
#define ER_FormatNumberTemplateRounding                -1009
#define ER_FormatNumberTemplateNoDecimal               -1010
#define ER_FormatNumberTemplateDecimal2                -1011
#define ER_FormatNumberTemplateDecimal3                -1012
#define ER_FormatNumberTemplateBuflen                  -1013
#define ER_FormatNumberTemplateNoDigit                 -1014
#define ER_FormatNumberTemplateSomeKindOfError         -1015

  }