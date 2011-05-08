#include "blocktypes.h"

struct BlockType blocktypes[256] = {
    [0] = {.name = "air",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT},
    [1] = {.name = "stone",
           .texcoords = {1, 0}},
    [2] = {.name = "grass",
           .texcoords = {3, 0},
           .texfunc = NULL}, //TODO: color top
    [3] = {.name = "dirt",
           .texcoords = {2, 0}},
    [4] = {.name = "cobblestone",
           .texcoords = {0, 1}},
    [5] = {.name = "wooden plank",
           .texcoords = {4, 0}},
    [6] = {.name = "sapling",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {15, 0},
           .texfunc = NULL}, //TODO
    [7] = {.name = "bedrock",
           .texcoords = {1, 1}},
    [8] = {.name = "water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
           .texcoords = {13, 12}, //TODO: check
           .texfunc = NULL}, //TODO
    [9] = {.name = "stationary water",
           .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 12}}, //TODO: check
    [10] = {.name = "lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 14}, //TODO: check
            .texfunc = NULL}, //TODO
    [11] = {.name = "stationary lava",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {13, 14}}, //TODO: check
    [12] = {.name = "sand",
            .texcoords = {2, 1}},
    [13] = {.name = "gravel",
            .texcoords = {3, 1}},
    [14] = {.name = "gold ore",
            .texcoords = {2, 0}},
    [15] = {.name = "iron ore",
            .texcoords = {2, 1}},
    [16] = {.name = "coal ore",
            .texcoords = {2, 2}},
    [17] = {.name = "wood",
            .texcoords = {4, 1},
            .texfunc = NULL}, //TODO
    [18] = {.name = "leaves",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {4, 3},
            .texfunc = NULL}, //TODO
    [20] = {.name = "glass",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 3}},
    [21] = {.name = "lapis lazuli ore",
            .texcoords = {0, 10}},
    [22] = {.name = "lapis lazuli block",
            .texcoords = {0, 9}},
    [23] = {.name = "dispenser",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {14, 2},
            .texfunc = NULL}, //TODO
    [24] = {.name = "sandstone",
            .texcoords = {0, 11}, //TODO
            .texfunc = NULL}, //TODO
    [25] = {.name = "note block",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {10, 3}},
    [26] = {.name = "bed",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texcoords = {6, 8},
            .texfunc = NULL}, //TODO
    [27] = {.name = "powered rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 11},
            .texfunc = NULL}, // TODO
    [28] = {.name = "detector rail",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {3, 12},
            .texfunc = NULL}, // TODO
    [35] = {.name = "wool",
            .texcoords = {0, 4}, //TODO
            .texfunc = NULL}, //TODO
    [37] = {.name = "dandelion",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 0},
            .texfunc = NULL}, //TODO: for orientation
    [38] = {.name = "rose",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 0},
            .texfunc = NULL}, //TODO: for orientation
    [39] = {.name = "brown mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {13, 1},
            .texfunc = NULL}, //TODO: for orientation
    [40] = {.name = "red mushroom",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texcoords = {12, 1},
            .texfunc = NULL}, //TODO: for orientation
    [41] = {.name = "gold block",
            .texcoords = {7, 1}},
    [42] = {.name = "iron block",
            .texcoords = {6, 1}},
    [43] = {.name = "double slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [44] = {.name = "slabs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [45] = {.name = "brick block",
            .texcoords = {7, 0}},
    [46] = {.name = "tnt",
            .texfunc = NULL}, //TODO
    [47] = {.name = "bookshelf",
            .texfunc = NULL}, //TODO
    [48] = {.name = "moss stone",
            .texcoords = {4, 2}},
    [49] = {.name = "obsidian",
            .texcoords = {5, 2}},
    [50] = {.name = "torch",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [51] = {.name = "fire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [52] = {.name = "monster spawner",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 4}}, //TODO
    [53] = {.name = "wooden stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [54] = {.name = "chest",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 1},
            .texfunc = NULL}, //TODO
    [55] = {.name = "redstone wire",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [56] = {.name = "diamond ore",
            .texcoords = {2, 3}},
    [57] = {.name = "diamond block",
            .texcoords = {8, 1}},
    [58] = {.name = "crafting table",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {11, 2},
            .texfunc = NULL}, //TODO
    [59] = {.name = "seeds",
            .texcoords = {13, 5}, //TODO
            .texfunc = NULL}, //TODO
    [60] = {.name = "farmland",
            .texcoords = {6, 5}, //TODO
            .texfunc = NULL}, //TODO
    [61] = {.name = "furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {12, 2},
            .texfunc = NULL}, //TODO: share function with dispenser
    [62] = {.name = "burning furnace",
            .flags = BLOCKTYPE_FLAG_USABLE,
            .texcoords = {13, 3},
            .texfunc = NULL}, //TODO: share function with dispenser
    [63] = {.name = "sign post",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [64] = {.name = "wooden door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {1, 5},
            .texfunc = NULL}, //TODO
    [65] = {.name = "ladders",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [66] = {.name = "rails",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [67] = {.name = "cobblestone stairs",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [68] = {.name = "wall sign",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [69] = {.name = "lever",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [70] = {.name = "stone pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [71] = {.name = "iron door",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [72] = {.name = "wooden pressure plate",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [73] = {.name = "redstone ore",
            .texcoords = {3, 3}},
    [74] = {.name = "glowing redstone ore",
            .texcoords = {3, 3}}, //TODO
    [75] = {.name = "redstone torch (off)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [76] = {.name = "redstone torch (on)",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [77] = {.name = "stone button",
            .flags = BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texfunc = NULL}, //TODO
    [78] = {.name = "snow",
           .texcoords = {4, 4},
           .texfunc = NULL}, //TODO: share with grass
    [79] = {.name = "ice",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT,
           .texcoords = {3, 4}},
    [80] = {.name = "snow block",
            .texcoords = {2, 4}},
    [81] = {.name = "cactus",
            .texcoords = {5, 4},
            .texfunc = NULL}, //TODO: share with trees?
    [82] = {.name = "clay block",
            .texcoords = {8, 4}},
    [83] = {.name = "sugar cane",
            .flags = BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK | BLOCKTYPE_FLAG_TRANSPARENT,
            .texcoords = {9, 4},
            .texfunc = NULL}, //TODO: share with flowers/mushrooms?
    [84] = {.name = "jukebox",
            .texcoords = {11, 4},
            .texfunc = NULL}, //TODO: share with trees?
    [85] = {.name = "fence",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [86] = {.name = "pumpkin",
            .texfunc = NULL}, //TODO
    [87] = {.name = "netherrack",
            .texcoords = {7, 6}},
    [88] = {.name = "soul sand",
            .texcoords = {8, 6}},
    [89] = {.name = "glowstone block",
            .texcoords = {9, 6}},
    [90] = {.name = "portal",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK,
            .texfunc = NULL}, //TODO
    [91] = {.name = "jack-o-lantern",
            .texfunc = NULL}, //TODO: share with pumpkin
    [92] = {.name = "cake block",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE,
            .texfunc = NULL}, //TODO
    [93] = {.name = "redstone repeater (off)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    [94] = {.name = "redstone repeater (on)",
            .flags = BLOCKTYPE_FLAG_TRANSPARENT | BLOCKTYPE_FLAG_USABLE | BLOCKTYPE_FLAG_NONSOLID | BLOCKTYPE_FLAG_NONBLOCK, //TODO: check
            .texfunc = NULL}, //TODO
    //... TODO...
};
