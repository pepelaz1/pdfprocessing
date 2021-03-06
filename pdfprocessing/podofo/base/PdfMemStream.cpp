/***************************************************************************
 *   Copyright (C) 2007 by Dominik Seichter                                *
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

#include "PdfMemStream.h"

#include "PdfArray.h"
#include "PdfEncrypt.h"
#include "PdfFilter.h"
#include "PdfObject.h"
#include "PdfOutputDevice.h"
#include "PdfOutputStream.h"
#include "PdfVariant.h"
#include "PdfDefinesPrivate.h"

#include <stdlib.h>

namespace PoDoFo {

PdfMemStream::PdfMemStream( PdfObject* pParent )
    : PdfStream( pParent ), m_pStream( NULL ), m_pBufferStream( NULL ), m_lLength( 0 )
{
}

PdfMemStream::PdfMemStream( const PdfMemStream & rhs )
    : PdfStream( NULL ), m_pStream( NULL ), m_pBufferStream( NULL ), m_lLength( 0 )
{
    operator=(rhs);
}

PdfMemStream::~PdfMemStream()
{
}

void PdfMemStream::BeginAppendImpl( const TVecFilters & vecFilters )
{
    m_buffer  = PdfRefCountedBuffer();
	m_lLength = 0;

    if( vecFilters.size() )
    {
        m_pBufferStream = new PdfBufferOutputStream( &m_buffer );
        m_pStream       = PdfFilterFactory::CreateEncodeStream( vecFilters, m_pBufferStream );
    }
    else 
        m_pStream = new PdfBufferOutputStream( &m_buffer );

}

void PdfMemStream::AppendImpl( const char* pszString, size_t lLen )
{
    m_pStream->Write( pszString, lLen );
}

void PdfMemStream::EndAppendImpl()
{
    if( m_pStream ) 
    {
        m_pStream->Close();

        if( !m_pBufferStream ) 
            m_lLength = dynamic_cast<PdfBufferOutputStream*>(m_pStream)->GetLength();

        delete m_pStream;
        m_pStream = NULL;
    }

    if( m_pBufferStream ) 
    {
        m_pBufferStream->Close();
        m_lLength = m_pBufferStream->GetLength();
        delete m_pBufferStream;
        m_pBufferStream = NULL;
    }

    if( m_pParent )
        m_pParent->GetDictionary().AddKey( PdfName::KeyLength, PdfVariant(static_cast<pdf_int64>(m_lLength) ) );
}

void PdfMemStream::GetCopy( char** pBuffer, pdf_long* lLen ) const
{
    if( !pBuffer || !lLen )
    {
        PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
    }

    *pBuffer = static_cast<char*>(malloc( sizeof( char ) * m_lLength ));
    *lLen    = m_lLength;
    
    if( !*pBuffer )
    {
        PODOFO_RAISE_ERROR( ePdfError_OutOfMemory );
    }
    
    memcpy( *pBuffer, m_buffer.GetBuffer(), m_lLength );
}


void PdfMemStream::GetCopy(PdfOutputStream * pStream) const
{
	if( !pStream)
	{
		PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
	}
	pStream->Write(m_buffer.GetBuffer(), m_lLength);
}

void PdfMemStream::Compress()
{
	PdfObject*        pObj;
    PdfVariant        vFilter( PdfName("FlateDecode" ) );
    PdfVariant        vFilterList;
    PdfArray          tFilters;

    PdfArray::const_iterator tciFilters;
    
    if( !m_lLength )
        return; // ePdfError_ErrOk

    // TODO: Handle DecodeParms
    if( m_pParent->GetDictionary().HasKey( "Filter" ) )
    {
     /*   pObj = m_pParent->GetIndirectKey( "Filter" );

        if( pObj->IsName() )
        {
			PdfName nm = pObj->GetName();
            if( pObj->GetName() != "DCTDecode" && pObj->GetName() != "FlateDecode" )
            {
                tFilters.push_back( vFilter );
                tFilters.push_back( *pObj );
            }
        }
        else if( pObj->IsArray() )
        {
            tciFilters = pObj->GetArray().begin();

            while( tciFilters != pObj->GetArray().end() )
            {
                if( (*tciFilters).IsName() )
                {
                    // do not compress DCTDecoded are already FlateDecoded streams again
                    if( (*tciFilters).GetName() == "DCTDecode" || (*tciFilters).GetName() == "FlateDecode" )
                    {
                        return;
                    }
                }

                ++tciFilters;
            }

            tFilters.push_back( vFilter );

            tciFilters = pObj->GetArray().begin();

            while( tciFilters != pObj->GetArray().end() )
            {
                tFilters.push_back( (*tciFilters) );
                
                ++tciFilters;
            }
        }
        else
            return;

        vFilterList = PdfVariant( tFilters );
        m_pParent->GetDictionary().AddKey( "Filter", vFilterList );

		CompressStreamData(); // throws an exception on error*/
    }
    else
    {
       m_pParent->GetDictionary().AddKey( "Filter", PdfName( "FlateDecode" ) );
       CompressStreamData();
    }
}

void PdfMemStream::CompressStreamData()
{
    char*            pBuffer;
    pdf_long             lLen;

    if( !m_lLength )
        return;

    std::auto_ptr<PdfFilter> pFilter = PdfFilterFactory::Create( ePdfFilter_FlateDecode );
	if( pFilter.get() )
    {
        pFilter->Encode( m_buffer.GetBuffer(), m_buffer.GetSize(), &pBuffer, &lLen );
        this->Set( pBuffer, lLen );
    }
    else
    {
        PODOFO_RAISE_ERROR( ePdfError_UnsupportedFilter );
    }
}


void PdfMemStream::FlateCompress()
{
    PdfObject*        pObj;
    PdfVariant        vFilter( PdfName("FlateDecode" ) );
    PdfVariant        vFilterList;
    PdfArray          tFilters;

    PdfArray::const_iterator tciFilters;
    
    if( !m_lLength )
        return; // ePdfError_ErrOk

    // TODO: Handle DecodeParms
    if( m_pParent->GetDictionary().HasKey( "Filter" ) )
    {
        pObj = m_pParent->GetIndirectKey( "Filter" );

        if( pObj->IsName() )
        {
            if( pObj->GetName() != "DCTDecode" && pObj->GetName() != "FlateDecode" )
            {
                tFilters.push_back( vFilter );
                tFilters.push_back( *pObj );
            }
        }
        else if( pObj->IsArray() )
        {
            tciFilters = pObj->GetArray().begin();

            while( tciFilters != pObj->GetArray().end() )
            {
                if( (*tciFilters).IsName() )
                {
                    // do not compress DCTDecoded are already FlateDecoded streams again
                    if( (*tciFilters).GetName() == "DCTDecode" || (*tciFilters).GetName() == "FlateDecode" )
                    {
                        return;
                    }
                }

                ++tciFilters;
            }

            tFilters.push_back( vFilter );

            tciFilters = pObj->GetArray().begin();

            while( tciFilters != pObj->GetArray().end() )
            {
                tFilters.push_back( (*tciFilters) );
                
                ++tciFilters;
            }
        }
        else
            return;

        vFilterList = PdfVariant( tFilters );
        m_pParent->GetDictionary().AddKey( "Filter", vFilterList );

        FlateCompressStreamData(); // throws an exception on error
    }
    else
    {
        m_pParent->GetDictionary().AddKey( "Filter", PdfName( "FlateDecode" ) );
        FlateCompressStreamData();
    }
}

void PdfMemStream::Uncompress()
{
    pdf_long         lLen;
    char*        pBuffer = NULL;
    
    TVecFilters  vecEmpty;

    if( m_pParent && m_pParent->IsDictionary() && m_pParent->GetDictionary().HasKey( "Filter" ) && m_lLength )
    {
        try {
            this->GetFilteredCopy( &pBuffer, &lLen );
        } catch( PdfError & e ) {
            if( pBuffer )
                free( pBuffer );

            throw e;
        }

        this->Set( pBuffer, lLen, vecEmpty );
        // free the memory allocated by GetFilteredCopy again.
        podofo_free( pBuffer );

        m_pParent->GetDictionary().RemoveKey( "Filter" ); 
        if( m_pParent->GetDictionary().HasKey( "DecodeParms" ) ) 
        {
            m_pParent->GetDictionary().RemoveKey( "DecodeParms" ); 
        }
    }
}

void PdfMemStream::FlateCompressStreamData()
{
    char*            pBuffer;
    pdf_long             lLen;

    if( !m_lLength )
        return;

    std::auto_ptr<PdfFilter> pFilter = PdfFilterFactory::Create( ePdfFilter_FlateDecode );
	if( pFilter.get() )
    {
        pFilter->Encode( m_buffer.GetBuffer(), m_buffer.GetSize(), &pBuffer, &lLen );
        this->Set( pBuffer, lLen );
    }
    else
    {
        PODOFO_RAISE_ERROR( ePdfError_UnsupportedFilter );
    }
}

const PdfStream & PdfMemStream::operator=( const PdfStream & rhs )
{
    const PdfMemStream* pStream = dynamic_cast<const PdfMemStream*>(&rhs);
    if( pStream )
        m_buffer = pStream->m_buffer;
    else
        return PdfStream::operator=( rhs );

    m_lLength = rhs.GetLength();
    if( m_pParent ) 
        m_pParent->GetDictionary().AddKey( PdfName::KeyLength, PdfVariant( static_cast<pdf_int64>(m_lLength) ) );

    return *this;
}

void PdfMemStream::Write( PdfOutputDevice* pDevice, PdfEncrypt* pEncrypt ) 
{
    pDevice->Print( "stream\n" );
    if( pEncrypt ) 
    {
        char* pBuffer;
        pdf_long  lLen;
        this->GetCopy( &pBuffer, &lLen );

        pdf_long nOutputLen = pEncrypt->CalculateStreamLength(lLen);
        pdf_long nOffset = pEncrypt->CalculateStreamOffset();

        char *pOutputBuffer = new char[nOutputLen+1];
        memcpy(&pOutputBuffer[nOffset], pBuffer, lLen);

        pEncrypt->Encrypt( reinterpret_cast<unsigned char*>(pOutputBuffer), lLen );
        pDevice->Write( pOutputBuffer, nOutputLen );

        delete[] pOutputBuffer;
        free( pBuffer );                               
    }
    else
    {
        pDevice->Write( this->Get(), this->GetLength() );
    }
    pDevice->Print( "\nendstream\n" );
}

pdf_long PdfMemStream::GetLength() const
{
    return m_lLength;
}


};
