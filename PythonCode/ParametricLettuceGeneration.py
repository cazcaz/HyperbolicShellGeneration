from matplotlib import pyplot as plt
import numpy as np
from scipy.optimize import minimize
from numpy.random import rand
from mpl_toolkits.mplot3d import Axes3D
from math import sin, cos, pi, sinh, tan, acos, sqrt
from stl import mesh
import os

class EnergyFunction:
    def __init__(self, curve, normals, binormals, length, radialDist, initDist, lengthPunishment = 100, stiffness = 0.05):
        self.curve = curve
        self.normals = normals
        self.binormals = binormals
        self.length = length
        self.stiffness = stiffness
        self.lengthEnergyCoef = lengthPunishment
        self.radialDist = radialDist
        self.initialDist = initDist
        self.resolution = 0

    def normaliseVector(self, vector):
        norm = np.sqrt(np.dot(vector,vector))
        if norm >= 1e-6:
            return vector / np.sqrt(np.dot(vector,vector))
        else:
            return np.zeros(3)

    def energyFunction(self, inputs):
        #the inputs are the displacements to be iterated
        self.resolution = len(inputs)
        
        #Reinitialise constants
        curve = self.curve
        normals = self.normals
        binormals = self.binormals
        length = self.length
        stiffness = self.stiffness
        resolution = self.resolution
        lengthPunishment = self.lengthEnergyCoef
        radialDist = self.radialDist
        initialRadius = self.initialDist


        circumferentialEnergySum = 0
        totalLength = 0
        for i in range(resolution):
            #phi[i-1][j+1]
            p1 = self.decompExpansion(curve[self.ind(i-1)], normals[self.ind(i-1)], binormals[self.ind(i-1)], length, inputs[self.ind(i-1)])
            #phi[i][j+1]
            p2 = self.decompExpansion(curve[self.ind(i)], normals[self.ind(i)], binormals[self.ind(i)], length, inputs[self.ind(i)])
            #phi[i+1][j+1]
            p3 = self.decompExpansion(curve[self.ind(i+1)], normals[self.ind(i+1)], binormals[self.ind(i+1)], length, inputs[self.ind(i+1)])

            #Sometimes computation leads to a value that is very very close to 1 or -1, usually out of the bounds of acos(x) by 1/10^8, so move values to 1 or -1 here
            cosAngle = np.dot(self.normaliseVector(p1 - p2), self.normaliseVector(p3 - p2))
            if cosAngle > 1:
                cosAngle = 1
            elif cosAngle < -1:
                cosAngle = -1
            
            #bending energy circumferentially
            circumferentialEnergySum +=  1/(self.norm(p1-p2) + self.norm(p3-p2)) * tan(acos(cosAngle))**2

            #arclength of the circumference
            totalLength += self.norm(p2 - p1)

        #sum of energies around the boundary for bending in the radial direction
        radialEnergySum = sum([(inputs[i]/length)**2 for i in range(resolution)])

        #difference of total length and desired length squared
        lengthEnergy = lengthPunishment * (totalLength - self.lengthFunction(radialDist, initialRadius))**2

        #sum of all energies
        totalEnergy = stiffness*circumferentialEnergySum + stiffness/(2*length) * radialEnergySum
        return totalEnergy
        #return lengthEnergy 
    
    def energyFunctionDerivative(self, inputs):
        #the inputs are the displacements to be iterated
        self.resolution = len(inputs)
        
        #reinitialise constants
        curve = self.curve
        normals = self.normals
        binormals = self.binormals
        length = self.length
        stiffness = self.stiffness
        resolution = self.resolution
        lengthCoef = self.lengthEnergyCoef
        radialDist = self.radialDist
        initialRadius = self.initialDist

        derivatives = []
        
        #arclength of the boundary (made compact with list comprehension)
        arclength = sum([self.norm(self.decompExpansion(curve[self.ind(i)], normals[self.ind(i)], binormals[self.ind(i)], length, inputs[self.ind(i)]) - self.decompExpansion(curve[self.ind(i-1)], normals[self.ind(i-1)], binormals[self.ind(i-1)], length, inputs[self.ind(i-1)])) for i in range(resolution)])
        
        for i in range(resolution):
            #for each derivative, terms come from 3 different angles, and in turn 5 different vectors on the curve

            #phi[i-2][j+1]
            p1 = self.decompExpansion(curve[self.ind(i-2)], normals[self.ind(i-2)], binormals[self.ind(i-2)], length, inputs[self.ind(i-2)])
            #phi[i-1][j+1]
            p2 = self.decompExpansion(curve[self.ind(i-1)], normals[self.ind(i-1)], binormals[self.ind(i-1)], length, inputs[self.ind(i-1)])
            #phi[i][j+1]
            p3 = self.decompExpansion(curve[self.ind(i)], normals[self.ind(i)], binormals[self.ind(i)], length, inputs[self.ind(i)])
            #phi[i+1][j+1]
            p4 = self.decompExpansion(curve[self.ind(i+1)], normals[self.ind(i+1)], binormals[self.ind(i+1)], length, inputs[self.ind(i+1)])
            #phi[i+2][j+1]
            p5 = self.decompExpansion(curve[self.ind(i+2)], normals[self.ind(i+2)], binormals[self.ind(i+2)], length, inputs[self.ind(i+2)])

            #derivatives of each w.r.t. d_ij
            dp1 = np.zeros(3)
            dp2 = np.zeros(3)
            dp3 = binormals[i]
            dp4 = np.zeros(3)
            dp5 = np.zeros(3)

            #The x of the tan^2(acos(x)) function, and its derivative
            x1 = np.dot(self.normaliseVector(p5-p4), self.normaliseVector(p3-p4))
            xdij1 = np.dot(self.normaliseVector(p5-p4), self.differentiateNormalVector(p4,p3,dp4,dp3)) + np.dot(self.normaliseVector(p3-p4), self.differentiateNormalVector(p5,p4,dp5,dp4))
            
            x2 = np.dot(self.normaliseVector(p4-p3), self.normaliseVector(p2-p3))
            xdij2 = np.dot(self.normaliseVector(p4-p3), self.differentiateNormalVector(p3,p2,dp3,dp2)) + np.dot(self.normaliseVector(p2-p3), self.differentiateNormalVector(p4,p3,dp4,dp3))
            
            x3 = np.dot(self.normaliseVector(p3-p2), self.normaliseVector(p1-p2))
            xdij3 = np.dot(self.normaliseVector(p3-p2), self.differentiateNormalVector(p2,p1,dp2,dp1)) + np.dot(self.normaliseVector(p1-p2), self.differentiateNormalVector(p3,p2,dp3,dp2))
            
            #Sum of all contributing terms, and the addition of the radial bending energy derivative
            dijDerivBend = self.dxdij(x1, xdij1, p3,p4,p5,dp3,dp4,dp5,stiffness) + self.dxdij(x2, xdij2,p2,p3,p4,dp2,dp3,dp4 ,stiffness) + self.dxdij(x3, xdij3, p1,p2,p3,dp1,dp2,dp3,stiffness)

            #Derivative of the arclength energy contribution
            lengthEnergyDeriv =  - 2 * lengthCoef * (arclength - self.lengthFunction(radialDist, initialRadius)) * (np.dot(dp4-dp3, self.normaliseVector(p4-p3)) + np.dot(dp3-dp2, self.normaliseVector(p3-p2)))
            
            #All summed together to form dE/ddij
            derivatives.append(lengthEnergyDeriv + dijDerivBend + stiffness/length**3 * inputs[i])
            #derivatives.append(lengthEnergyDeriv)
        return derivatives

    def differentiateNormalVector(self, a, b, da, db):
        #returns the derivative of a-b/||a-b||, as this is found a lot
        norm = self.norm(a-b)
        if norm == 0:
            return np.zeros(3)
        result = (da - db)/norm - (np.dot(da-db,a-b)/norm**3) * (a - b)
        return result

    def norm(self, vector):
        result = np.sqrt(np.dot(vector,vector))
        if result < 1e-5:
            return 0
        return result

    def dxdij(self, x, xdij, p1, p2, p3, dp1, dp2, dp3, stiffness):
        norm1 = self.norm(p3-p2)
        norm2 = self.norm(p2-p1)
        if norm1 == 0 or norm2 == 0:
            return 0
        if x <= 1e-6:
            return 0
        return stiffness*(-2/(norm1 + norm2) * xdij/x**3 - (np.dot(dp3-dp2,p3-p2)/norm1 + np.dot(dp1-dp2,p1-p2)/norm2)*(1/x**2 - 1))
    
    def decompExpansion(self, curvePoint, normal, binormal, length, input):
        return curvePoint + length * normal + input * binormal
    
    def lengthFunction(self, t, t0):
        desiredCurvature = 2 #keep pos, will be assumed to be neg.
        #adjustment = 2*pi*(t0 - 1/sqrt(desiredCurvature)*sinh(sqrt(desiredCurvature)*t0))
        return 2*pi/sqrt(desiredCurvature)*sinh(sqrt(desiredCurvature)*t)
    
    def ind(self, index):
        resolution = self.resolution
        if index >= resolution:
            return self.ind(index - resolution)
        elif index < 0:
            return self.ind(index + resolution)
        return index

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

    def expandCurve(self, length, stiffness=0.05, lengthPunish = 100):
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
        
        #A set of tangents that stay constant throughout the expansion, which are the tangents to the inintial circle
        tangents= [self.normaliseVector([-sin(2*pi*i/resolution), cos(2*pi*i/resolution), 0]) for i in range(resolution)]
        
        #tangents = [self.normaliseVector(prevCurve[self.ind(i+1)]-prevCurve[self.ind(i-1)]) for i in range(resolution)]
        binormals = [self.normaliseVector(np.cross(curveNormals[i],tangents[i])) for i in range(len(curveNormals))]
        radialDistance = self.norm(self.curves[0][0]) + (len(self.curves) - 1) * length
        iterableEnergy = EnergyFunction(prevCurve, curveNormals, binormals, length, self.norm(self.curves[0][0]), radialDistance, lengthPunish, stiffness)
        d_min = -0.05
        d_max = 0.05
        pt = d_min + rand(resolution) * (d_max-d_min)
        #pt = np.zeros(resolution)
        result = minimize(iterableEnergy.energyFunction, pt, method='L-BFGS-B', jac=iterableEnergy.energyFunctionDerivative)
        solution = result['x']
        #print('Total Evaluations: %d' % result['nfev'])
        self.curves.append([prevCurve[i] + length * curveNormals[i] + binormals[i] * solution[i] for i in range(resolution)])
        
    def expandCurveNTimes(self, iterations, length, stiffness, lengthPunish = 100, saveIntermittent = False):
        for k in range(iterations):
            self.expandCurve(length, stiffness, lengthPunish)
            print("Curve " + str(len(self.curves)) + " found.")
            if k>1 and k%100 == 0 and saveIntermittent:
                self.saveAsMesh("Iteration " + str(k))
                print("Saved intermittent curve")

    def normaliseVector(self, vector):
        norm = np.sqrt(np.dot(vector,vector))
        if norm >= 1e-6:
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

    def norm(self, vector):
        result = np.sqrt(np.dot(vector,vector))
        if result < 1e-5:
            return 0
        return result

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

    def ind(self, index):
        if len(self.curves) < 1:
            print('No curves present.')
            return 0
        resolution = len(self.curves[0])
        if index >= resolution:
            return self.ind(index - resolution)
        elif index < 0:
            return self.ind(index + resolution)
        return index
    
    def testEnergyDeriv(self, testInput, lengthPunish,stiffness):
        if len(self.curves) < 1:
            print('Not enough curves initialised.')
            return 0
        self.expandCurveNTimes(5,1,0,0)
        resolution = len(self.curves[0])
        curveNormals = [self.normaliseVector(self.curves[-1][i]-self.curves[-2][i]) for i in range(resolution)]
        tangents= [self.normaliseVector(self.curves[-1][self.ind(i+1)]-self.curves[-1][self.ind(i-1)]) for i in range(resolution)]
        binormals = [self.normaliseVector(np.cross(curveNormals[i],tangents[i])) for i in range(len(curveNormals))]
        iterableEnergy = EnergyFunction(self.curves[0], curveNormals, binormals, 1, 7, lengthPunish, stiffness)
        h = 0.0000000005
        movedIndex = resolution//2
        testInputMoved = testInput.copy()
        testInputMoved[movedIndex] += h
        print((iterableEnergy.energyFunction(testInput) - iterableEnergy.energyFunction(testInputMoved))/h) 
        print(iterableEnergy.energyFunctionDerivative(testInput)[movedIndex])

    
if __name__ == "__main__":
    lettuce = CurveGenerator("Circular")
    initialCircle = CircleCurveGenerator(1000,40,[0,0,0])
    lettuce.setInitCurve(initialCircle.getCurve())
    lettuce.expandCurveNTimes(100,1,10000,0)
    lettuce.saveAsMesh("LP0")
    
    

    
    

