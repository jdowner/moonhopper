
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

	void LoadPNG( const std::string& filename, InternalTexture& texture );
	void SavePNG( const std::string& filename, const InternalTexture& texture );

	void CreateTextureObject( InternalTexture& texture ) const;
	void SwapRedAndBlueBits( InternalTexture& texture ) const;

public:
	~TextureManager();

	bool LoadTexture( const std::string& filename, const std::string& textureID );
	void UnloadTexture( const std::string& textureID );
	void UnbindTextures();
	void DeleteTextures();

	int NumberOfTextures() const;

	unsigned int GetTextureHandle( const std::string& name );

	Texture* GetTexture( const std::string& name );
	Texture* GetTexture( int index );
		
};

#endif
