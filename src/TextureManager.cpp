
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <GL/gl.h>
#include <png.h>

#include "TextureManager.h"
#include "Texture.h"

namespace
{
	const int PNG_HEADER_LENGTH = 8; // Define the header length of a PNG file
}

//
//	Destructor
//
TextureManager::~TextureManager()
{
	deleteTextures();
}

//
//	Loads the texture from file. The texture data is kept and
//	associated with the filename, so filenames must be unique
//	to the textures used.
//
bool TextureManager::loadTexture( const std::string& filename, const std::string& textureID )
{
	// If this name is already in the manager, the texture is not loaded
	if ( m_map.end() == m_map.find( textureID ) )
	{
		// Create a new texture object
		InternalTexture* texture = new InternalTexture;

		// Load the image
		loadPNG( filename, *texture );

		// Create an object that OpenGL can work with
		createTextureObject( *texture );

		// Store pointer to the texture
		m_map[ textureID ] = texture;

	}
	return true;
}

//
//	Removes the texture from the manager
//
void TextureManager::unloadTexture( const std::string& /*textureName*/ )
{
}

//
//	Returns the number of textures stored in the manager
//
int TextureManager::numberOfTextures() const
{
	return (int)m_map.size();
}

//
//	Returns a pointer to the named texture
//
Texture* TextureManager::getTexture( const std::string& name )
{
	TextureMap::iterator texture = m_map.find( name );
	return ( m_map.end() != texture) ? ( texture->second ) : ( NULL );
}

//
//	Returns a pointer to indicated texture
//
Texture* TextureManager::getTexture( int index )
{
	if ( (int)m_map.size() <= index )
	{
		return NULL;
	}

	TextureMap::const_iterator it = m_map.begin();
	for ( int i = 0; i < index; ++i, ++it );

	return it->second;
}

//
//	Generates OpenGL texture objects for each image
//
void TextureManager::createTextureObject( InternalTexture& texture ) const
{
	// Check the dimensions of the texture
	if ( !texture.height || !texture.width )
		throw std::runtime_error("Textures must have non-zero dimensions");

	if ( ( ( texture.height & ( texture.height - 1 ) ) != 0 ) ||
		( ( texture.width & ( texture.width - 1 ) ) != 0 ) )
		throw std::runtime_error("Texture dimensions must be a power of two");

	glGenTextures( 1, &(texture.handle) );
	glBindTexture(GL_TEXTURE_2D, texture.handle);

	// minimum required to set the min and mag texture filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// now that the texture object is bound, specify a texture for it
	switch ( texture.bytes )
	{
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, &texture.data.front() );
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height,
			0, GL_RGB, GL_UNSIGNED_BYTE, &texture.data.front() );
		break;

	}
}

//
//	Swaps the red bits with the blue bits
//
void TextureManager::swapRedAndBlueBits( InternalTexture& texture ) const
{
	const int pixels = texture.height * texture.width;
	const unsigned int chunk = texture.bytes;
	for ( int i = 0; i < pixels; ++i )
	{
		const TextureDataType data = texture.data[ i * chunk ];
		texture.data[ i * chunk ] = texture.data[ i * chunk + 2 ];
		texture.data[ i * chunk + 2 ] = data;
	}
}

//
//	Remove all of the textures from the OpenGL state machine.
//
void TextureManager::unbindTextures()
{
	for ( TextureMap::iterator texture = m_map.begin(); texture != m_map.end(); ++texture )
	{
		glDeleteTextures( 1, &(texture->second->handle) );
	}
}

//
//	Destroys all textures
//
void TextureManager::deleteTextures()
{
	// Make sure that the textures are unbound from OpenGL
	unbindTextures();

	// Now we can safely delete the textures
	for ( TextureMap::iterator texture = m_map.begin(); texture != m_map.end(); ++texture )
	{
		delete texture->second;
	}

	// Erase the map
	m_map.clear();

}

//
//	Returns the handle of the requested texture
//
unsigned int TextureManager::getTextureHandle( const std::string& name ) const
{
	TextureMap::const_iterator texture = m_map.find( name );
	if ( m_map.end() == texture )
	{
		throw std::runtime_error("ERROR: Unable to find requested texture");
	}

	return texture->second->handle;
}

//
//	Loads a PNG file into a texture
//
void TextureManager::loadPNG( const std::string& filename, InternalTexture& texture )
{
	unsigned char header[PNG_HEADER_LENGTH];
	FILE *fp;

	// Open the file and check that it has the expected header
	if ( ( fp = fopen( filename.c_str(), "rb" ) ) == NULL )
		throw std::runtime_error("Unable to open file");

	fread( header, 1, PNG_HEADER_LENGTH, fp );

	if (png_sig_cmp(header, 0, PNG_HEADER_LENGTH))
	{
		fclose( fp );
		throw std::runtime_error("This is not a PNG file");
	}

	// Create and initialize png_struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL );
	if ( !png_ptr )
	{
		fclose( fp );
		throw std::runtime_error("Unable to create png_ptr");
	}

	// Create and initialize png_info. This contains the image information.
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if ( !info_ptr )
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		throw std::runtime_error("Unable to create info_ptr");
	}

	// Something to do with error handling that I don't understand
	if ( setjmp( png_jmpbuf( png_ptr ) ) )
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose( fp );
		throw std::runtime_error("Problem occured when calling setjmp()");
	}

	// Initialize IO
	png_init_io(png_ptr, fp);

	// Tell the library that we've already read the header
	png_set_sig_bytes(png_ptr, PNG_HEADER_LENGTH);

	// Read in the whole image
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	// We have the data, so close the file
	fclose( fp );

	// Set the member values
	texture.bytes = png_get_channels(png_ptr, info_ptr);
	texture.width = png_get_image_width( png_ptr, info_ptr );
	texture.height = png_get_image_height( png_ptr, info_ptr );

	// Allocate memory for image data
	png_bytep* row_pointers = new png_bytep[ texture.height ];
	row_pointers = png_get_rows( png_ptr, info_ptr );

	// Ensure the data container is empty
	texture.data.clear();

	// Copy the data
	const int row_length = texture.width * texture.bytes;
	for ( int i = texture.height; --i >= 0; /*nothing*/ )
	{
		texture.data.insert( texture.data.end(), row_pointers[ i ], row_pointers[ i ] + row_length );
	}

	delete row_pointers;
}

//
//	Saves a PNG file from a texture
//
void TextureManager::savePNG( const std::string& filename, const InternalTexture& texture )
{
	// Open file to write to
	FILE *fp = fopen( filename.c_str(), "wb");
	if (!fp)
	{
		throw std::runtime_error("Unable to open/create file");
	}

	// Create the png struct
	png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		throw std::runtime_error("Unable to create write struct");
	}

	// Create the info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
	   png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	   throw std::runtime_error("Unable to create info struct");
	}

	// Set the jmp to handle any errors in libpng
	if (setjmp(png_jmpbuf(png_ptr)))
	{
	   png_destroy_write_struct(&png_ptr, &info_ptr);
	   fclose(fp);
	   throw std::runtime_error("Unable to setjmp correctly");
	}

	// Initialize the IO
	png_init_io(png_ptr, fp);

	// Create row pointers to the texture data
	png_bytepp row_pointers = new png_bytep[ texture.height ];
	for ( unsigned int i = 0; i < texture.height; ++i )
	{
		row_pointers[i] = (png_bytep)( &texture.data.front() + ( texture.height - i - 1 ) * texture.width * texture.bytes );
	}

	png_set_rows( png_ptr, info_ptr, row_pointers );

	// Create the PNG header
	png_set_IHDR(png_ptr, info_ptr, texture.width, texture.height, 8, PNG_COLOR_TYPE_RGB_ALPHA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	// Create the PNG file
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	// Cleanup
	png_destroy_write_struct( &png_ptr, &info_ptr );

	// Close the file
	fclose( fp );

	// Delete the row pointers
	delete row_pointers;

}
