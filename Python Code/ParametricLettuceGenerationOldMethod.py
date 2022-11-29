from matplotlib import pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from math import sin, cos, pi
from stl import mesh
import os

class CircleCurveGenerator:
    def __init__(self, resolution, radius, centre) -> None:
        self.resolution = resolution
        self.radius = radius
        self.centre = centre
        pass

    def getCurve(self):
        curve = []
        for i in range(self.resolution):
            t = 2*pi/self.resolution * i
            curve.append(np.array([self.centre[0] + self.radius * cos(t),self.centre[1] + self.radius * sin(t),self.centre[2]]))
        return curve

    def setResolution(self, resolution):
        self.resolution = resolution

    def setRadius(self, radius):
        self.radius = radius

    def setCentre(self, centre):
        self.centre = centre

class CurveGenerator:
    def __init__(self, shape) -> None:
        self.shape = shape
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111, projection='3d')
        self.ax.set_box_aspect([1,1,1])
        self.curves = []

    def getCurveType(self):
        print(self.shape)
    
    def getShape(self):
        return self.shape

    def plotFigures(self):
        for curve in self.curves:
            xs,ys,zs = self.plottableForm(curve)
            plt.plot(xs,ys,zs)
        
    def showPlot(self):
        plt.axis('off')
        plt.show()
    
    def setInitCurve(self, curve):
        if self.curves:
            self.curves.clear()
        self.curves.append(curve)

    def expandCurve(self, length, amplitude, phase):
        if len(self.curves) == 0:
            print("No curve initialised")
            return
        resolution = len(self.curves[0])
        prevCurve = self.curves[-1]
        if len(self.curves) == 1:
            if self.shape == "Circular":
                curveNormals = [self.normaliseVector(direction) for direction in self.curves[0]]
            if self.shape == "Straight":
                curveNormals = [np.array([0,1,0]) for _ in self.curves[0]]
        else:
            curveNormals = [self.normaliseVector(prevCurve[i]-self.curves[-2][i]) for i in range(resolution)]
        tangents = [self.normaliseVector(prevCurve[1]-prevCurve[-1])]
        for i in range(1,resolution-1):
            tangents.append(self.normaliseVector(prevCurve[i+1]-prevCurve[i-1]))
        tangents.append(self.normaliseVector(prevCurve[0]-prevCurve[-2]))
        binormals = [self.normaliseVector(np.cross(curveNormals[i],tangents[i])) for i in range(len(curveNormals))]
        self.curves.append([prevCurve[i] + curveNormals[i] * length + amplitude * cos(phase * 2*pi*i/resolution) * binormals[i] for i in range(resolution)])

    def expandCurveNTimes(self,iterations,length,amplitude,phase):
        for _ in range(iterations):
            self.expandCurve(length,amplitude,phase)

    def normaliseVector(self, vector):
        norm = np.sqrt(np.dot(vector,vector))
        if not norm == 0:
            return vector / np.sqrt(np.dot(vector,vector))
        else:
            return np.zeros(3)

    def plottableForm(self, curve):
        xs,ys,zs = [],[],[]
        for i in range(1,len(curve)):
            xs.append(curve[i][0])
            ys.append(curve[i][1])
            zs.append(curve[i][2])
        xs.append(xs[0])
        ys.append(ys[0])
        zs.append(zs[0])
        return xs,ys,zs

    def plotVector(self,point,vector):
        xs = [point[0], point[0] + vector[0]]
        ys = [point[1], point[1] + vector[1]]
        zs = [point[2], point[2] + vector[2]]
        plt.plot(xs,ys,zs,'r')

    def saveAsMesh(self, name):
        if len(self.curves) <= 1:
            print('Not enough curves initialised. Cannot save a mesh from no triangles.')
            return
        resolution = len(self.curves[0])
        listVertices = []
        for curve in self.curves:
            for point in curve:
                listVertices.append(point.tolist())
        listTriangles = []
        for curveIndex in range(len(self.curves)-1):
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
        lettuceMesh.save(name + '.stl')


if __name__ == "__main__":
    lettuce = CurveGenerator("Circular")
    initialCircle = CircleCurveGenerator(1000,0.4,[0,0,1])
    lettuce.setInitCurve(initialCircle.getCurve())
    lettuce.expandCurveNTimes(5,2,0,0)
    lettuce.expandCurveNTimes(18,2,0.07,5)
    lettuce.plotFigures()
    lettuce.showPlot()
    lettuce.saveAsMesh('testFlower')

