#include "uniformBuffer.hpp"

void UniformBuffer::bindAll(Shader& shader) const {
	for (auto it : getInstanceData()) {
		if (!shader.hasUniform(it.first))
			continue;

		const ShaderUniform& uniformData = shader.getUniform(it.first);
		if (it.second.m_glType != uniformData.m_glType)
			continue;

		const void* const data = it.second.m_data;
		switch (it.second.m_glType) {
			case GL_FLOAT:
				glUniform1f(uniformData.m_location, *(float*)data);
				break;
			case GL_FLOAT_VEC2:
				glUniform2f(uniformData.m_location, ((Vector2*)data)->x(), ((Vector2*)data)->y());
				break;
			case GL_FLOAT_VEC3:
				glUniform3f(uniformData.m_location, ((Vector3*)data)->x(), ((Vector3*)data)->y(), ((Vector3*)data)->z());
				break;
			case GL_FLOAT_MAT4:
				glUniformMatrix4fv(uniformData.m_location, 1, GL_FALSE, ((Matrix4*)data)->getData());
				break;

			default:
				BEING_ERROR("Unsupported uniform type, soz.");
		}
	}
}
