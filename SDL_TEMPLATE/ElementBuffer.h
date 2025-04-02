#pragma once
class ElementBuffer {
private:
	unsigned int rendererID;
	unsigned int count;

public:
	ElementBuffer(const unsigned int* data, unsigned int pCount);
	~ElementBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return count; }
};

