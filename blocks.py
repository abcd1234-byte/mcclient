A = (0, 1)
B = (1, 1)
C = (1, 0)
D = (0, 0)


class FaceDirections:
    BOTTOM, TOP, EAST, WEST, NORTH, SOUTH = range(6)
    directions = ((0, -1, 0), (0, 1, 0), (0, 0, -1), (0, 0, 1), (-1, 0, 0), (1, 0, 0))



class FaceOrientations:
    WEST, NORTH, EAST, SOUTH = range(4)
    faces = {WEST: FaceDirections.WEST,
             NORTH: FaceDirections.NORTH,
             EAST: FaceDirections.EAST,
             SOUTH: FaceDirections.SOUTH}
    uvmaps = {NORTH: (D, A, B, C),
              EAST:  (C, D, A, B),
              SOUTH: (B, C, D, A),
              WEST:  (A, B, C, D)}



_block_types = {}
class BlockMeta(type):
    def __new__(mcs, name, bases, classdict):
        cls = type.__new__(mcs, name, bases, classdict)
        if 'id' in classdict:
            _block_types[classdict['id']] = cls
        return cls


class Block:
    __metaclass__ = BlockMeta
    transparent = False

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (0, 14, FaceOrientations.NORTH)

    @classmethod
    def from_type(cls, type):
        return _block_types.get(type, cls)



class Stone(Block):
    id = 1

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (1, 0, FaceOrientations.NORTH)


class Grass(Block):
    id = 2

    @classmethod
    def get_face_texture(cls, metadata, face):
        if face == FaceDirections.BOTTOM:
            return (2, 0, FaceOrientations.NORTH)
        elif face == FaceDirections.TOP:
            return (1, 9, FaceOrientations.NORTH) #TODO
        else:
            return (3, 0, FaceOrientations.NORTH)


class Dirt(Block):
    id = 3

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (2, 0, FaceOrientations.NORTH)


class Cobblestone(Block):
    id = 4

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (0, 1, FaceOrientations.NORTH)


class WoodenPlank(Block):
    id = 5

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (4, 0, FaceOrientations.NORTH)


class Gravel(Block):
    id = 13

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (3, 1, FaceOrientations.NORTH)


class IronOre(Block):
    id = 15

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (1, 2, FaceOrientations.NORTH)


class CoalOre(Block):
    id = 16

    @classmethod
    def get_face_texture(cls, metadata, face):
        return (2, 2, FaceOrientations.NORTH)


class Wood(Block):
    id = 17

    @classmethod
    def get_face_texture(cls, metadata, face):
        if face in (FaceDirections.TOP, FaceDirections.BOTTOM):
            return (5, 1, FaceOrientations.NORTH)
        else:
            if metadata == 0:
                return (4, 1, FaceOrientations.NORTH)
            elif metadata == 1:
                return (4, 7, FaceOrientations.NORTH)
            else:
                return (5, 7, FaceOrientations.NORTH)


class Bed(Block):
    id = 26

    transparent = True

    @classmethod
    def get_face_texture(cls, metadata, face):
        orientation = metadata & 3
        if metadata & 0x8:
            #TODO
            if face == FaceDirections.TOP:
                return (7, 8, orientation)
            elif FaceOrientations.faces[orientation] == face: #TODO
                return (8, 9, FaceOrientations.NORTH) #TODO
            else:
                return (7, 9, FaceOrientations.NORTH) #TODO
        else:
            #TODO
            if face == FaceDirections.TOP:
                return (6, 8, orientation)
            elif FaceOrientations.faces[(orientation + 2) % 4] == face: #TODO
                return (5, 9, FaceOrientations.NORTH) #TODO
            else:
                return (6, 9, FaceOrientations.NORTH) #TODO


class Torch(Block):
    id = 50
    #TODO

    transparent = True

    @classmethod
    def get_face_texture(cls, metadata, face):
        #TODO: transparent:
        return (4, 11, FaceOrientations.NORTH)


class CraftingTable(Block):
    id = 58

    @classmethod
    def get_face_texture(cls, metadata, face):
        if face == FaceDirections.TOP:
            return (11, 2, FaceOrientations.NORTH)
        else:
            var = {FaceDirections.EAST: 0,
                   FaceDirections.NORTH: 1,
                   FaceDirections.WEST: 0,
                   FaceDirections.SOUTH: 1}[face]
            return (11 + var, 3, FaceOrientations.NORTH)


class Furnace(Block):
    id = 61

    @classmethod
    def get_face_texture(cls, metadata, face):
        ok_face = metadata
        if face == ok_face:
            return (12, 2, FaceOrientations.NORTH)
        elif face == FaceDirections.TOP:
            return (14, 3, FaceOrientations.NORTH)
        else:
            return (13, 2, FaceOrientations.NORTH)


class BurningFurnace(Block):
    id = 62

    @classmethod
    def get_face_texture(cls, metadata, face):
        ok_face = metadata
        if face == ok_face:
            return (13, 3, FaceOrientations.NORTH)
        elif face == FaceDirections.TOP:
            return (14, 3, FaceOrientations.NORTH)
        else:
            return (13, 2, FaceOrientations.NORTH)



class WoodenDoor(Block):
    id = 64
    #TODO

    transparent = True

    @classmethod
    def get_face_texture(cls, metadata, face):
        half = metadata & 0x8
        open = metadata & 0x4
        corner = metadata & 3

        #TODO: transparent:
        return (4, 11, FaceOrientations.NORTH)
