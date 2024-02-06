#include "core/inc/predefined.h"


const u8 fh_ps_num = 3;
char *fh_ps_lst[3][3] = {
	{"flat", "res/predef/shaders/flat.vert", "res/predef/shaders/flat.frag"},
	{"ui", "res/predef/shaders/ui.vert", "res/predef/shaders/ui.frag"},
	{"model", "res/predef/shaders/model.vert", "res/predef/shaders/model.frag"}
};


/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *		DEFAULT BLOCK SHADER
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

char *fh_ps_shd_def_block_v = 
"#version 420 core\n"
"layout(location=0) in vec3 in_position;"
"layout(location=1) in ivec3 in_index;"
"layout(location=2) in int in_type;"
""
""
"uniform ivec2   u_frame;"
"uniform ivec4   u_rect[200];"
"uniform vec4    u_color[200];"
"uniform ivec4   u_radius[200];"
"uniform int     u_bwidth[200];"
"uniform vec4    u_bcolor[200];"
"uniform ivec2	 u_scroll[200];"
"uniform ivec4	 u_limit[200];"
""
"flat out vec2   fs_frame;"
"flat out int    fs_type;"
"out vec4        fs_color;"
"flat out vec4   fs_sides;"
"flat out vec4	 fs_limits;"
"flat out vec2   fs_cpoints[4];"
"flat out ivec4  fs_radius;"
"flat out int    fs_bwidth;"
"flat out vec4   fs_bcolor;"
"flat out ivec2  fs_scroll;"
""
"void main() {"
"	vec2 lframe = u_frame * 0.5;"
"	ivec4 lrect = u_rect[in_index.x];"
"	ivec4 llimits = ivec4(0, 0, u_frame.x, u_frame.y);"

"	if(in_index.y != -1) {"
"		llimits = u_limit[in_index.y];"
"	}"
""
"	fs_frame = u_frame;"
"	fs_type = in_type;"
"	fs_sides = vec4(lrect.y, lrect.x+lrect.z, lrect.y+lrect.w, lrect.x);"
"	fs_limits = vec4(llimits.y, llimits.x+llimits.z, llimits.y+llimits.w, llimits.x);"
""
"	switch(in_type) {"
"		case 1:"	/* default block */
"			ivec4 lradius = u_radius[in_index.z];"
"			fs_color = u_color[in_index.z];"
"			fs_cpoints = vec2[4]("
"				vec2(lrect.x + lradius[0], lrect.y + lradius[0]),"
"				vec2(lrect.x + lrect.z - lradius[1], lrect.y + lradius[1]),"
"				vec2(lrect.x + lrect.z - lradius[2], lrect.y + lrect.w - lradius[2]),"
"				vec2(lrect.x + lradius[3], lrect.y + lrect.w - lradius[3])"
"			);"
"			fs_radius = lradius;"
"			fs_bwidth = u_bwidth[in_index.z];"
"			fs_bcolor = u_bcolor[in_index.z];"
"			fs_scroll = ivec2(0, 0);"
"			break;"
"		case 2:"	/* vertical scrollbar */
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = u_scroll[in_index.z];"
"			break;"
"		case 3:"	/* horizontal scrollbar */
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = u_scroll[in_index.z];"
"			break;"
"		default:"
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = ivec2(0, 0);"
"			break;"
"	}"
""
"	gl_Position = vec4((in_position.x - lframe.x) / lframe.x, (lframe.y - in_position.y) / lframe.y, 1, 1);"
"}";

char *fh_ps_shd_def_block_f = 
"#version 420 core\n"
""
"flat in vec2    fs_frame;"
"flat in int     fs_type;"
"in vec4         fs_color;"
"flat in vec4    fs_sides;"
"flat in vec4	 fs_limits;"
"flat in vec2    fs_cpoints[4];"
"flat in ivec4   fs_radius;"
"flat in int     fs_bwidth;"
"flat in vec4    fs_bcolor;"
"flat in ivec2   fs_scroll;"
""
"out vec4 out_color;"
""
"void main() {"
"	int corner = -1;"
"	vec2 loc = vec2(gl_FragCoord.x, fs_frame.y - gl_FragCoord.y);"
"	float alpha = 1.0;"
"	vec4 color;"
"	float dist;"
""
"	int scrollbar_width = 10;"
"	int scroll_thumbsize = 8;"
"	int scroll_end_padding = 1;"
"	int scroll_side_padding = (scrollbar_width - scroll_thumbsize) / 2;"
""
"	if(loc.y < fs_limits[0] || loc.x > fs_limits[1] || loc.y > fs_limits[2] || loc.x < fs_limits[3]) {"
"		discard;"
"	}"
"	else {"
"		switch(fs_type) {"
"			case 1:"
"				if(loc.x <= fs_cpoints[0].x && loc.y <= fs_cpoints[0].y) corner = 0;"
"				else if(loc.x >= fs_cpoints[1].x && loc.y <= fs_cpoints[1].y) corner = 1;"
"				else if(loc.x >= fs_cpoints[2].x && loc.y >= fs_cpoints[2].y) corner = 2;"
"				else if(loc.x <= fs_cpoints[3].x && loc.y >= fs_cpoints[3].y) corner = 3;"
""
"				if(corner < 0) {"
"					if(loc.y <= fs_sides[0] + fs_bwidth || loc.x >= fs_sides[1] - fs_bwidth || loc.y >= fs_sides[2] - fs_bwidth || loc.x <= fs_sides[3] + fs_bwidth) {"
"						color = fs_bcolor;"
"					}"
"					else {"
"						color = fs_color;"
"					}"
"				}"
"				else {"
"					dist = distance(loc, fs_cpoints[corner]);"
"					if(dist >= fs_radius[corner]) alpha = 0;"
"					else if(dist >= (fs_radius[corner] - fs_bwidth)) color = fs_bcolor;"
"					else color = fs_color;"
"				}"
"				out_color = vec4(color.xyz, color.w * alpha);"
"				break;"
""
"			case 2:"
"				if(loc.y > fs_sides[0] + scroll_end_padding + fs_scroll[0] && loc.x < fs_sides[1] - scroll_side_padding && loc.y < fs_sides[0] + scroll_end_padding + fs_scroll[0] + fs_scroll[1] && loc.x > fs_sides[3] + scroll_side_padding) {"
"					out_color = vec4(1, 0, 1, 1);"
"				}"
"				else {"
"					out_color = vec4(1, 1, 0, 1);"
"				}"
"				break;"
"			case 3:"
"				if(loc.y > fs_sides[0] + scroll_side_padding && loc.x < fs_sides[3] + scroll_end_padding + fs_scroll[0] + fs_scroll[1] && loc.y < fs_sides[2] - scroll_side_padding && loc.x > fs_sides[3] + scroll_end_padding + fs_scroll[0]) {"
"					out_color = vec4(1, 0, 1, 1);"
"				}"
"				else {"
"					out_color = vec4(1, 1, 0, 1);"
"				}"
"				break;"
"			default:"
"				out_color = vec4(1, 0, 0, 1);"
"				break;"
"		}"
"	}"
"};";



/*
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 *
 *		DEFAULT TEXTURE SHADER
 *
 * -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
*/

char *fh_ps_shd_def_texture_v = 
"#version 420 core\n"
"layout(location=0) in vec3 in_position;"
"layout(location=1) in ivec3 in_index;"
"layout(location=2) in int in_type;"
""
""
"uniform ivec2   u_frame;"
"uniform ivec4   u_rect[200];"
"uniform vec4    u_color[200];"
"uniform ivec4   u_radius[200];"
"uniform int     u_bwidth[200];"
"uniform vec4    u_bcolor[200];"
"uniform ivec2	 u_scroll[200];"
"uniform ivec4	 u_limit[200];"
""
"flat out vec2   fs_frame;"
"flat out int    fs_type;"
"out vec4        fs_color;"
"flat out vec4   fs_sides;"
"flat out vec4	 fs_limits;"
"flat out vec2   fs_cpoints[4];"
"flat out ivec4  fs_radius;"
"flat out int    fs_bwidth;"
"flat out vec4   fs_bcolor;"
"flat out ivec2  fs_scroll;"
""
"void main() {"
"	vec2 lframe = u_frame * 0.5;"
"	ivec4 lrect = u_rect[in_index.x];"
"	ivec4 llimits = ivec4(0, 0, u_frame.x, u_frame.y);"

"	if(in_index.y != -1) {"
"		llimits = u_limit[in_index.y];"
"	}"
""
"	fs_frame = u_frame;"
"	fs_type = in_type;"
"	fs_sides = vec4(lrect.y, lrect.x+lrect.z, lrect.y+lrect.w, lrect.x);"
"	fs_limits = vec4(llimits.y, llimits.x+llimits.z, llimits.y+llimits.w, llimits.x);"
""
"	switch(in_type) {"
"		case 1:"	/* default block */
"			ivec4 lradius = u_radius[in_index.z];"
"			fs_color = u_color[in_index.z];"
"			fs_cpoints = vec2[4]("
"				vec2(lrect.x + lradius[0], lrect.y + lradius[0]),"
"				vec2(lrect.x + lrect.z - lradius[1], lrect.y + lradius[1]),"
"				vec2(lrect.x + lrect.z - lradius[2], lrect.y + lrect.w - lradius[2]),"
"				vec2(lrect.x + lradius[3], lrect.y + lrect.w - lradius[3])"
"			);"
"			fs_radius = lradius;"
"			fs_bwidth = u_bwidth[in_index.z];"
"			fs_bcolor = u_bcolor[in_index.z];"
"			fs_scroll = ivec2(0, 0);"
"			break;"
"		case 2:"	/* vertical scrollbar */
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = u_scroll[in_index.z];"
"			break;"
"		case 3:"	/* horizontal scrollbar */
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = u_scroll[in_index.z];"
"			break;"
"		default:"
"			fs_color = vec4(0, 0, 0, 1);"
"			fs_cpoints = vec2[4](vec2(0,0),vec2(0,0),vec2(0,0),vec2(0,0));"
"			fs_radius = ivec4(0, 0, 0, 0);"
"			fs_bwidth = 0;"
"			fs_bcolor = vec4(0.0, 0, 0, 1.0);"
"			fs_scroll = ivec2(0, 0);"
"			break;"
"	}"
""
"	gl_Position = vec4((in_position.x - lframe.x) / lframe.x, (lframe.y - in_position.y) / lframe.y, 1, 1);"
"}";

char *fh_ps_shd_def_texture_f = 
"#version 420 core\n"
""
"flat in vec2    fs_frame;"
"flat in int     fs_type;"
"in vec4         fs_color;"
"flat in vec4    fs_sides;"
"flat in vec4	 fs_limits;"
"flat in vec2    fs_cpoints[4];"
"flat in ivec4   fs_radius;"
"flat in int     fs_bwidth;"
"flat in vec4    fs_bcolor;"
"flat in ivec2   fs_scroll;"
""
"out vec4 out_color;"
""
"void main() {"
"	int corner = -1;"
"	vec2 loc = vec2(gl_FragCoord.x, fs_frame.y - gl_FragCoord.y);"
"	float alpha = 1.0;"
"	vec4 color;"
"	float dist;"
""
"	int scrollbar_width = 10;"
"	int scroll_thumbsize = 8;"
"	int scroll_end_padding = 1;"
"	int scroll_side_padding = (scrollbar_width - scroll_thumbsize) / 2;"
""
"	if(loc.y < fs_limits[0] || loc.x > fs_limits[1] || loc.y > fs_limits[2] || loc.x < fs_limits[3]) {"
"		discard;"
"	}"
"	else {"
"		switch(fs_type) {"
"			case 1:"
"				if(loc.x <= fs_cpoints[0].x && loc.y <= fs_cpoints[0].y) corner = 0;"
"				else if(loc.x >= fs_cpoints[1].x && loc.y <= fs_cpoints[1].y) corner = 1;"
"				else if(loc.x >= fs_cpoints[2].x && loc.y >= fs_cpoints[2].y) corner = 2;"
"				else if(loc.x <= fs_cpoints[3].x && loc.y >= fs_cpoints[3].y) corner = 3;"
""
"				if(corner < 0) {"
"					if(loc.y <= fs_sides[0] + fs_bwidth || loc.x >= fs_sides[1] - fs_bwidth || loc.y >= fs_sides[2] - fs_bwidth || loc.x <= fs_sides[3] + fs_bwidth) {"
"						color = fs_bcolor;"
"					}"
"					else {"
"						color = fs_color;"
"					}"
"				}"
"				else {"
"					dist = distance(loc, fs_cpoints[corner]);"
"					if(dist >= fs_radius[corner]) alpha = 0;"
"					else if(dist >= (fs_radius[corner] - fs_bwidth)) color = fs_bcolor;"
"					else color = fs_color;"
"				}"
"				out_color = vec4(color.xyz, color.w * alpha);"
"				break;"
""
"			case 2:"
"				if(loc.y > fs_sides[0] + scroll_end_padding + fs_scroll[0] && loc.x < fs_sides[1] - scroll_side_padding && loc.y < fs_sides[0] + scroll_end_padding + fs_scroll[0] + fs_scroll[1] && loc.x > fs_sides[3] + scroll_side_padding) {"
"					out_color = vec4(1, 0, 1, 1);"
"				}"
"				else {"
"					out_color = vec4(1, 1, 0, 1);"
"				}"
"				break;"
"			case 3:"
"				if(loc.y > fs_sides[0] + scroll_side_padding && loc.x < fs_sides[3] + scroll_end_padding + fs_scroll[0] + fs_scroll[1] && loc.y < fs_sides[2] - scroll_side_padding && loc.x > fs_sides[3] + scroll_end_padding + fs_scroll[0]) {"
"					out_color = vec4(1, 0, 1, 1);"
"				}"
"				else {"
"					out_color = vec4(1, 1, 0, 1);"
"				}"
"				break;"
"			default:"
"				out_color = vec4(1, 0, 0, 1);"
"				break;"
"		}"
"	}"
"};";
