from stl import mesh
import os
import numpy as np

surfaces = []
fileNames = []
current = os.getcwd()
count = 0
os.chdir("..")
os.chdir(current + "\\OutputSurfaceTxts")
for filename in os.listdir(os.getcwd()):
    if not filename.endswith('.txt'):
        continue
    fileNames.append(filename[:-4])
    with open(os.path.join(os.getcwd(), filename), 'r') as f:
        currentFile = f.readlines()
        curvesStrs = [line[:-2] for line in currentFile]
        curves = []
        for curveString in curvesStrs:
            pointStrs = curveString.split(" ")
            curve = []
            for pointString in pointStrs:
                point = [float(value) for value in pointString.split(",")]
                curve.append(point)
            curves.append(curve)
    surfaces.append(curves)

os.chdir("..")
current = os.getcwd()
os.chdir(current + "\\OutputSurfaceMeshes")
for surface, fileName in zip(surfaces, fileNames):
    count += 1
    resolution = len(surface[0])
    listVertices = []
    for curve in surface:
        for point in curve:
            listVertices.append(point)
    listTriangles = []
    for curveIndex in range(len(surface)-1):
        startPoint = resolution*curveIndex
        for pointIndex in range(resolution-1):
            listTriangles.append([startPoint + pointIndex, startPoint + pointIndex + resolution + 1,startPoint + pointIndex + 1])
            listTriangles.append([startPoint + pointIndex, startPoint + pointIndex + resolution, startPoint + pointIndex + resolution+1])
        listTriangles.append([startPoint + resolution - 1,  startPoint + resolution,startPoint])
        listTriangles.append([startPoint + resolution - 1, startPoint + 2*resolution - 1, startPoint + resolution])
    triangles = np.array(listTriangles)
    vertices = np.array(listVertices)
    lettuceMesh = mesh.Mesh(np.zeros(triangles.shape[0], dtype=mesh.Mesh.dtype))
    for i, f in enumerate(triangles):
        for j in range(3):
            lettuceMesh.vectors[i][j] = vertices[f[j],:]
    lettuceMesh.save(fileName + '.stl')

print("Done, " , count , " surfaces converted.")