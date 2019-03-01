#pragma once

class VkGenerator
{
public:
	/* constructor/destructor */
	VkGenerator( ) = default;
	~VkGenerator( ) = default;

	/* copy */
	VkGenerator( const VkGenerator& _other ) = delete;
	void operator=( const VkGenerator& _other ) = delete;

	/* move */
	VkGenerator( const VkGenerator&& _other ) = delete;
	void operator=( const VkGenerator&& _other ) = delete;
private:
};
