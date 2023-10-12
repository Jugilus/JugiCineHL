#ifndef JMSAVESIGNATURES_H
#define JMSAVESIGNATURES_H


namespace jugimap {


namespace SaveSignature
{

    static const int FORMAT = 88880;
    static const int FORMAT_VERSION = 1;

    static const int EXPORTED_MAP_FORMAT = 99990;
    static const int EXPORTED_MAP_FORMAT_VERSION = 2;

    static const int EXPORTED_SOURCE_GRAPHICS_FORMAT = 100000;
    static const int EXPORTED_SOURCE_GRAPHICS_FORMAT_VERSION = 2;

    static const int HEADER = 100;
    static const int HEADER_V2 = 101;
    static const int HEADER_V3 = 102;
    static const int HEADER_V4 = 103;
    static const int HEADER_V5 = 104;
    static const int MISC_DATA = 110;
    static const int THUMBNAIL = 120;

    static const int VARIABLES = 200;
    static const int VARIABLES_V2 = 201;
    static const int VARIABLES_V3 = 202;
    static const int CHILD_ITEMS = 210;
    static const int PARAMETERS = 220;
    static const int CONSTANT_PARAMETERS = 230;
    static const int PARALLAX_LAYER_PARAMETERS = 240;
    static const int SCREEN_LAYER_PARAMETERS = 250;

    static const int SOURCE_DATA_SAVE_HELPERS = 300;
    static const int SOURCE_SETS = 300;
    static const int SOURCE_SET = 310;
    static const int SOURCE_OBJECTS = 330;
    static const int SOURCE_OBJECT = 340;


    static const int SOURCE_IMAGE_FILES = 400;
    static const int SOURCE_IMAGE_FILE = 410;
    static const int SOURCE_IMAGES = 420;
    static const int SOURCE_IMAGES_V2 = 421;
    static const int SOURCE_IMAGES_V3 = 422;
    static const int SOURCE_IMAGE = 430;

    static const int MAP = 500;
    static const int LAYERS = 510;
    static const int LAYERS_GROUP = 520;
    static const int LAYER = 530;
    static const int VECTOR_LAYERS_GROUP = 540;
    static const int VECTOR_LAYER = 550;
    static const int STATIC_LAYER = 560;

    static const int OBJECTS = 700;
    static const int OBJECTS_V2 = 701;
    static const int OBJECT = 710;
    static const int SOURCE_OBJECT_REPLACEMENTS = 720;
    static const int VECTOR_SHAPES = 750;
    static const int VECTOR_SHAPES_V2 = 751;
    static const int VECTOR_SHAPE = 760;
    static const int VECTOR_SHAPE_CONTROL_POINTS = 770;

    static const int COMPOSED_SPRITE = 800;

    static const int ANIMATIONS = 900;
    static const int FRAME_ANIMATIONS = 950;
    static const int FRAME_ANIMATION = 960;
    static const int ANIMATION_FRAMES = 970;

    static const int TIMELINE_ANIMATIONS = 1000;
    static const int TIMELINE_ANIMATION = 1010;
    static const int ANIMATION_MEMBERS = 1020;
    static const int ANIMATION_TRACK = 1030;
    static const int ANIMATION_KEYS = 1040;
    static const int AK_TRANSLATION = 1100;
    static const int AK_ROTATION = 1110;
    static const int AK_SCALING = 1120;
    static const int AK_FLIP_HIDE = 1130;
    static const int AK_ALPHA_CHANGE = 1140;
    static const int AK_OVERLAY_COLOR_CHANGE = 1150;
    static const int AK_PATH_MOVEMENT = 1160;
    static const int AK_FRAME_CHANGE = 1170;
    static const int AK_FRAME_ANIMATION = 1180;
    static const int AK_TIMELINE_ANIMATION = 1190;
    static const int AK_META_TRACK = 1250;



}



}


#endif // JMSAVESIGNATURES_H
