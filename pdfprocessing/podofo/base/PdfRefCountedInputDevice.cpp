/***************************************************************************
 *   Copyright (C) 2006 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "PdfRefCountedInputDevice.h"  

#include "PdfInputDevice.h"
#include "PdfDefinesPrivate.h"

namespace PoDoFo {

PdfRefCountedInputDevice::PdfRefCountedInputDevice()
    : m_pDevice( NULL )
{

}

PdfRefCountedInputDevice::PdfRefCountedInputDevice( const char* pszFilename, const char* )
    : m_pDevice( NULL )
{
    m_pDevice              = new TRefCountedInputDevice();
    m_pDevice->m_lRefCount = 1;

    try {
        m_pDevice->m_pDevice = new PdfInputDevice( pszFilename );
    } catch( const PdfError & rError ) {
        delete m_pDevice;
        throw rError;
    }
}

#ifdef _WIN32
#if defined(_MSC_VER)  &&  _MSC_VER <= 1200			// nicht f?r Visualstudio 6
#else
PdfRefCountedInputDevice::PdfRefCountedInputDevice( const wchar_t* pszFilename, const char* )
    : m_pDevice( NULL )
{
    m_pDevice              = new TRefCountedInputDevice();
    m_pDevice->m_lRefCount = 1;

    try {
        m_pDevice->m_pDevice = new PdfInputDevice( pszFilename );
    } catch( const PdfError & rError ) {
        delete m_pDevice;
        throw rError;
    }
}
#endif
#endif // _WIN32

PdfRefCountedInputDevice::PdfRefCountedInputDevice( const char* pBuffer, size_t lLen )
    : m_pDevice( NULL )
{
    m_pDevice              = new TRefCountedInputDevice();
    m_pDevice->m_lRefCount = 1;


    try {
        m_pDevice->m_pDevice   = new PdfInputDevice( pBuffer, lLen );
    } catch( const PdfError & rError ) {
        delete m_pDevice;
        throw rError;
    }
}

PdfRefCountedInputDevice::PdfRefCountedInputDevice( PdfInputDevice* pDevice )
    : m_pDevice( NULL )
{
    m_pDevice              = new TRefCountedInputDevice();
    m_pDevice->m_lRefCount = 1;
    m_pDevice->m_pDevice   = pDevice;
}

PdfRefCountedInputDevice::PdfRefCountedInputDevice( const PdfRefCountedInputDevice & rhs )
    : m_pDevice( NULL )
{
    this->operator=( rhs );
}

PdfRefCountedInputDevice::~PdfRefCountedInputDevice()
{
    Detach();
}

void PdfRefCountedInputDevice::Detach()
{
    if( m_pDevice && !--m_pDevice->m_lRefCount ) 
    {
        // last owner of the file!
        m_pDevice->m_pDevice->Close();
        delete m_pDevice->m_pDevice;
        delete m_pDevice;
        m_pDevice = NULL;
    }

}

const PdfRefCountedInputDevice & PdfRefCountedInputDevice::operator=( const PdfRefCountedInputDevice & rhs )
{
    Detach();

    m_pDevice = rhs.m_pDevice;
    if( m_pDevice )
        m_pDevice->m_lRefCount++;

    return *this;
}


};
