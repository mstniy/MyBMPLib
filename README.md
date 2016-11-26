# MyBMPLib

A super simple C++ bitmap library.

## Usage

#### "BCreateBitmap" function:
  Creates a new bitmap in the given dimensions and assigns it to the Bitmap object given in the parameter.
  Returns true if successful, false otherwise.

#### "BCloneBitmap" function:
  Creates a new bitmap identical to the second parameter and assigns it to the first parameter.
  Doesn't modify any of the bitmaps if they point to the very same object.
	Returns true if successful, false otherwise.
  
#### "BDeleteBitmap" function:
  Deletes the bitmap object in the parameter, releasing associated memory.
	Returns true if successful, false otherwise.

#### "BGetPixel" function:
  Returns the color of a pixel in the given bitmap.
  Returns a "COLORREF" (defined in windows.h).
	Returns "CLR_INVALID" if an invalid coordinate is specified.

#### "BSetPixel" function:
  Sets the color of a pixel in the given bitmap.
	Returns a "COLORREF" (the same as the value given to it)
	Returns "CLR_INVALID" if an invalid coordinate is specified.

#### "BExportToDDB" function:
  Copies the given bitmap into the given device-dependent-bitmap (DDB).
	Returns true if successful, false otherwise.

#### "BImportFromDDB" function:
  Copies the pixels of the given DDB into the given bitmap.
	Returns true if successful, false otherwise.

#### "BExportToFile" function:
  Exports the given bitmap into the file at the given path.
  Returns true if successful, false otherwise.

#### "BImportFromFile" function:
  Imports the bitmap file at the given path into the given bitmap.
	Supports only 24-bit and 32-bit bitmaps.
	Returns true if successful, false otherwise.
  
## Remarks
 
#### Using BGetPixel and BSetPixel:
 
  *For both functions, the X parameter represents the horizontal and the Y parameter represents the vertical.
   Upper left corner has coordinates (0,0).*
   
  *In order to create a copy of a bitmap, do not use the assignment operator (=). Instead, use BCloneBitmap function.*
