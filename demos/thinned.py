import bpy
import bmesh
from math import sqrt, pi, sin, cos

ring_verts = [288,288,288,288,288,288,288,288,284,284,284,280,280,280,276,276,272,272,268,264,264,260,256,256,252,248,244,240,236,236,232,228,224,220,216,212,204,200,196,192,188,184,176,172,168,164,156,152,148,140,136,128,124,116,112,108,100,96,88,84,76,72,64,60,52,44,40,32,28,20,16,8,4]
radius = 10
ringAmount = len(ring_verts)
baseAngle = (pi/2) / ringAmount

def get_diameter(z):
    return sqrt(radius**2 - z**2)

bm = bmesh.new()
for i,v in enumerate(ring_verts):
    angle = (i * baseAngle)
    z = radius * sin( angle )
    diam = get_diameter(z)
    ret = bmesh.ops.create_circle(bm, cap_ends=False, diameter=diam, segments=v)
    bmesh.ops.translate(bm, verts=ret['verts'], vec=(0.0,0.0,z))

me = bpy.data.meshes.new("Mesh")
bm.to_mesh(me)
bm.free()

scene = bpy.context.scene
obj = bpy.data.objects.new("Object", me)
scene.objects.link(obj)
scene.objects.active = obj
obj.select = True
