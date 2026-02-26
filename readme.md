# Tokyo Mirage Sessions ♯FE Level Data Documentation

## General Data

### .cpk
This is an archive format developed by CRI Middleware. All of TMS♯FE's files are stored in very large cpk files. They are loosely grouped into separate categories. Some useful ones:
- `pack_000_map.cpk` Level models
- `pack_010_character.cpk` Most other models, including characters, npcs, props, game objects
- `pack_999_lua.cpk` Lua scripts that spawn objects and npcs in levels

These can be opened using a variety of programs, see https://amicitia.miraheze.org/wiki/CPK

## Level files

### .apak
`binary_templates/apak.bt`

Short for Atlas Pack. All the files for a level are bundled together in a `model.apak` file. This format has a simple header that lists each file's name, start offset, and size. 

### .bfres
`binary_templates/bfres_switch.bt`

Models and animations are stored in bfres files. This format was developed by Nintendo and is used in a variety of Wii U and Switch games. There are many different versions.

TMS♯FE uses bfres files with version 0x03050002. Some unused levels use an earlier version, 0x03040002. The data is encoded in big endian.

TMS♯FE Encore uses bfres files with version 0x00090000. While the core format remains the same there are many differences between Wii U bfres files and Switch bfres files, the major one being that data is encoded in little endian. Also to note there are still unused levels in the game's files, but they remain in the Wii U bfres format. The file for f010_04 (Gongen Hot Springs) remains, but the model itself is empty.

bfres files support storing many types of data into a single file, however TMS♯FE only stores a single model per bfres file. The exception is particle effects, which have an embedded bfres file that contains multiple models.

Levels usually have a main model, stored in the bfres file named after the level's identifier, eg `d003_01.bfres`. There are some additional models, such as `sky.bfres`, and sub models usually named `obj1.bfres`, `obj2.bfres`, etc. These sub models can have animations attached to them, which are BFRES files but have a `.anm` file extension.

bfres files can include embedded files, which TMS♯FE uses to include a bntx file containing the model's textures.

there is also support for custom properties that can be assigned to some sections called user data. TMS♯FE uses this a lot, sometimes ignoring built in bfres features in favor of user data.

Data in a bfres file has a four letter character code to mark it's type. Levels make use of mainly three types:

#### FMDL
Short for Cafe Model, contains separate subsections that make up a 3D model:
- FVTX (Cafe Vertex)
- FSHP (Cafe Shape)
- FSKL (Cafe Skeleton)
- FMAT (Cafe Material)

FMATs have the following user data:
- spec_power (float)
- cull_mode (int32)
- blend_mode (int32)
- noLighting (int32)
- use_ibl (int32)
- ibl_scale (float)
- use_overlay (int32)
- s_lowlight (float[3])
- s_highlight (float[3])
- lowlightMin (float)
- lowlightMax (float)
- highlightMin (float)
- highlightMax (float)
- fresnel_scale (float)
- fresnel_width (float)
- fresnel_power (float)
- use_specular (int32)
- spec_scale (float)
- spec_shadow_scale (float)
- s_specmask (float[3])
- s_reflect (float[3])
- use_rimright (int32)
- rimlight_scale (float)
- rimlight_width (float)
- rimlight_power (float)
- rimlight_color (float[3])
- rimlight_alpha (float)
- use_rim_edge (int32)
- rim_edge_offset (float)
- rim_edge_power (float)
- rim_edge_color (float[3])
- rim_edge_alpha (float)
- use_glow (int32)
- glow_color (float[3])
- use_glare_effect (uint32)
- edge_width (float)
- use_edge_light_offset (int32)
- edge_light_scale (float)
- use_noise (int32)
- use_noise_ss (int32)
- noise_func (int32)
- noise_nrm_scale (float)
- noise_lum_scale (float)
- shadow_cast (int32)
- shadow_recieve (int32)
- shadow_self (int32)

Bones with meshes have the following user data:
- render_mesh_enable
- render_edge_enable
- render_shadow_cast
- render_shadow_recieve
- render_shadow_self
- render_nocolor

They are all int32s but store only 0 or 1

#### FSKA
Short for Cafe Skeletal Animation.

bfres files support a variety of animation curve types, but TMS♯FE only uses cubic animation curves. Every keyframe stores 4 coefficients that can be used to construct a cubic function. The values stored aren't the actual coefficients, in order to get the correct coefficients they all need to be multiplied by the curve's data scale value. the constant coefficient also has to add the curve's data offset value. Presumably storing the data like this allows for greater precision.

#### FMAA
Short for Cafe Material Animation.

These only contain texture scale rotation translation animations. Because animations are always stored in a separate bfres file from the model, the data that specifies which animation curve applies to which material and transformation is unused, and is instead specified in the user data. The key is the material name, and the value is an array of strings, odd strings specifying which texture and transformation, and even strings specifying the index of the animation curve.

#### bfres resources
note: these all apply to different versions of the file format
- https://mk8.tockdom.com/wiki/BFRES_(File_Format)
- https://epd.zeldamods.org/wiki/Bfres
- https://github.com/KillzXGaming/BfresLibrary
- https://github.com/KillzXGaming/Switch-Toolbox

### .atlm
`binary_templates/atlm.bt`

Short for Atlus Lightmap. Stores lightmap textures for a model. They have some data which specifies how to transform the uv coords for each bone of the model. These transformations use bottom left as 0, but the vertex buffers inside model files use top left as 0. There is a bntx file that contains the actual lightmap texture.

### .lig
`binary_templates/lig.bt`

Has point light and directional light definitions.

### maplayout.layout
`binary_templates/maplayout.bt`

These files contain 3d coordinates that can be used to spawn objects, npcs, define trigger volumes, etc. They are split into different groups, some of these groups are referenced by name in lua scripts:

0. LAYOUT_EVENT
1. LAYOUT_EVENT_DIR (most map lua scripts reference this group when spawning gimmicks)
2.
3. LAYOUT_OBJECT
4. enemy spawners
5. LAYOUT_NPC
6.
7. triggers to leave an area and open the tokyo map, also portals in dungeons
8. treasure chests
9. once sided breakable walls
10. ch5 breakable walls
11.
12. trigger areas
13. drop off ledges
14. LAYOUT_LADDER
15.
16. heal points
17. teleporters
18. idolasphere gates
19. stairs
20. LAYOUT_LIGHTSPOT (items that can be picked up off the ground for quests)
21.
22. doors
23.
24.
25. LAYOUT_TRAP
26.
27. stairs
28.
29. triple enemy spawners
30.
31. LAYOUT_TRANS_FLOOR
32.
33.
34.
35.
36.
37. blue treasure chests (area of aspirations)

These entries also contain what I assume are generic arguments, some integers some strings. This is used for gate entries to specify which model to use.

### .pspk
Short for Pixel Shader Pack. An apak file file containing `.pso` files.

TMS♯FE pixel shaders have Gfx2 as the first four bytes.

TMS♯FE Encore pixel shaders have BNSH as the first four bytes.

### .atcol
Short for Atlas Collision. Contains the collision mesh for the level.

### .stsp
Purpose currently unknown.

### path_info.stfl
Lists every file in the apak file.

### .lua
Lua scripts, which appear to be duplicates of ones found inside `pack_999_lua.cpk/Map`.
mapstaticeffect.lua has definitions for which particle effects to spawn in the map.
Most useful scripts are found in `pack_999_lua.cpk/Event/field/script_map`

Wii U .lua files can be decompiled with this site: https://luadec.metaworm.site/ 

## Tables
### symbolencountlayouttable.tbl
`binary_templates/symbolencountlayouttable.bt`

Located in `pack_000_map.cpk/Map`
Enemy symbols spawn from points that are defined in maplayout.layout. This table includes some data associated with each enemy spawner, and likely an index into another table for what set of enemies to use, but more research is required.

### gimmicktable.tbl
`binary_templates/gimmicktable.bt`

Located in `pack_000_map.cpk/Map`
Purpose currently unknown.
