
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "Texture.h"

class TextureManager
{
private:
	typedef unsigned char TextureDataType;
	typedef std::vector< TextureDataType > TextureDataContainer;
	struct InternalTexture : public Texture
	{
		TextureDataContainer data;
	};

private:
	typedef std::map< std::string, InternalTexture* > TextureMap;
	typedef std::map< unsigned int, TextureDataType* > TextureIdToDataMap;

private:
	TextureMap				m_map;
	TextureDataContainer	m_data;

	void loadPNG( const std::string& filename, InternalTexture& texture );
	void savePNG( const std::string& filename, const InternalTexture& texture );

	void createTextureObject( InternalTexture& texture ) const;
	void swapRedAndBlueBits( InternalTexture& texture ) const;

public:
	~TextureManager();

	bool loadTexture( const std::string& filename, const std::string& textureID );
	void unloadTexture( const std::string& textureID );
	void unbindTextures();
	void deleteTextures();

	int numberOfTextures() const;

	unsigned int getTextureHandle( const std::string& name ) const;

	Texture* getTexture( const std::string& name );
	Texture* getTexture( int index );

};

#endif
