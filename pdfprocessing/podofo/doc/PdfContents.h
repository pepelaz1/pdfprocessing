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

#ifndef _PDF_CONTENTS_H_
#define _PDF_CONTENTS_H_

#include "PdfDefines.h"
#include "PdfElement.h"

namespace PoDoFo {

class PdfObject;
class PdfPage;

/** A interface that provides a wrapper around "PDF content" -
	the instructions that are used to draw on the PDF "canvas".
 */
class PODOFO_DOC_API PdfContents : private PdfElement {
 public:

    /** Construct a new/empty set of contents in the owning objects
     */
    PdfContents( PdfDocument* pParent );

    /** Construct a new/empty set of contents in the owning objects
     */
    PdfContents( PdfVecObjects* pParent );

    /** Construct the contents from an existing PdfObject
     */
    PdfContents( PdfObject* inObj );

    /** Create the contents for an existing page which does not yet 
     *  have a contents object.
     *
     *  \param pParent a /Contents key will be added to this page 
     *         and a contents object will be created.
     */
    PdfContents( PdfPage* pParent );

    /** Virtual destructor - because ALL destructors should be...
     */
    virtual ~PdfContents() {};

    /** Get access to the raw contents object.
     *  It will either be a PdfStream or a PdfArray
     *  \returns a contents object
     */
    virtual PdfObject* GetContents() const { return mContObj; }

    /** Get access to an object into which you can add contents
     *   at the end of the "stream".
     */
    virtual PdfObject* GetContentsForAppending() const;

 private:
    PdfObject*	mContObj;
};

};

#endif /* _PDF_CONTENTS_H_ */
