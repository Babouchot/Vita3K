#pragma once

#include <crypto/hash.h>
#include <glutil/object.h>
#include <gxm/types.h>

#include "state.h"
#include "types.h"

#include <psp2/gxm.h>

#include <map>
#include <memory>
#include <string>
#include <tuple>

struct GxmContextState;
struct MemState;
struct FeatureState;

namespace renderer::gl {

// Compile program.
SharedGLObject compile_program(ProgramCache &program_cache, ShaderCache &vertex_cache, ShaderCache &fragment_cache,
    const GxmContextState &state, const FeatureState &features, const MemState &mem, const char *base_path, const char *title_id);

// Shaders.
std::string load_shader(const SceGxmProgram &program, const FeatureState &features, const char *base_path, const char *title_id);

// Uniforms.
bool set_uniform(GLuint program, const SceGxmProgram &shader_program, GLShaderStatics &statics, const MemState &mem,
    const SceGxmProgramParameter *parameter, const void *data, bool log_uniforms);

bool create(std::unique_ptr<Context> &context);
bool create(std::unique_ptr<RenderTarget> &rt, const SceGxmRenderTargetParams &params, const FeatureState &features);
bool create(std::unique_ptr<FragmentProgram> &fp, GLState &state, const SceGxmProgram &program, const emu::SceGxmBlendInfo *blend, GXPPtrMap &gxp_ptr_map, const char *base_path, const char *title_id);
bool create(std::unique_ptr<VertexProgram> &vp, GLState &state, const SceGxmProgram &program, GXPPtrMap &gxp_ptr_map, const char *base_path, const char *title_id);
bool sync_state(GLContext &context, const GxmContextState &state, const MemState &mem, bool enable_texture_cache);
void sync_rendertarget(const GLRenderTarget &rt);
void set_context(GLContext &ctx, const GLRenderTarget *rt, const FeatureState &features);
void get_surface_data(GLContext &context, size_t width, size_t height, size_t stride_in_pixels, uint32_t *pixels);
void draw(GLState &renderer, GLContext &context, GxmContextState &state, const FeatureState &features, SceGxmPrimitiveType type, SceGxmIndexFormat format,
    const void *indices, size_t count, const MemState &mem, const char *base_path, const char *title_id,
    const bool log_active_shaders, const bool log_uniforms);

void upload_vertex_stream(GLContext &context, const std::size_t stream_index, const std::size_t length, const void *data);

// State
void sync_viewport(const GxmContextState &state);
void sync_clipping(const GxmContextState &state);
void sync_cull(const GxmContextState &state);
void sync_front_depth_func(const GxmContextState &state);
void sync_front_depth_write_enable(const GxmContextState &state);
bool sync_depth_data(const GxmContextState &state);
bool sync_stencil_data(const GxmContextState &state);
void sync_stencil_func(const GxmContextState &state, bool is_back_stencil);
void sync_front_polygon_mode(const GxmContextState &state);
void sync_front_point_line_width(const GxmContextState &state);
void sync_front_depth_bias(const GxmContextState &state);
void sync_blending(const GxmContextState &state, const MemState &mem);
void sync_texture(GLContext &context, const GxmContextState &state, const MemState &mem, std::size_t index,
    bool enable_texture_cache);
void sync_vertex_attributes(GLContext &context, const GxmContextState &state, const MemState &mem);
void bind_fundamental(GLContext &context);

struct GLTextureCacheState;
struct TextureCacheState;

// Attribute formats.
size_t attribute_format_size(SceGxmAttributeFormat format);
GLenum attribute_format_to_gl_type(SceGxmAttributeFormat format);
GLboolean attribute_format_normalised(SceGxmAttributeFormat format);

namespace texture {

// Textures.
void bind_texture(GLTextureCacheState &cache, const emu::SceGxmTexture &gxm_texture, const MemState &mem);
void configure_bound_texture(const emu::SceGxmTexture &gxm_texture);
void upload_bound_texture(const emu::SceGxmTexture &gxm_texture, const MemState &mem);

// Texture formats.
const GLint *translate_swizzle(SceGxmTextureFormat fmt);
GLenum translate_internal_format(SceGxmTextureFormat src);
GLenum translate_format(SceGxmTextureFormat src);
GLenum translate_type(SceGxmTextureFormat format);
GLenum translate_wrap_mode(SceGxmTextureAddrMode src);
GLenum translate_minmag_filter(SceGxmTextureFilter src);
size_t bits_per_pixel(SceGxmTextureBaseFormat base_format);

// Texture cache.
bool init(GLTextureCacheState &cache);

} // namespace texture

} // namespace renderer::gl