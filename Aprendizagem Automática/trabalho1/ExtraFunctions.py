from Tree import Node
import csv
import numpy as np
import math
from copy import deepcopy


# Método que calcula e retorna a Razão do Erro
def errorReason(pi, n):
	minimo = 1
	if n == 0:
		return minimo
	for i in pi:
		if (i/n) < minimo:	
			minimo = (i/n)
	return minimo


# Método que calcula e retorna o Índice de Gini
def gini(pi, n):
	impureza = 1
	if n == 0:
		return impureza
	for i in pi:
		impureza -= pow(i/n , 2)
	return impureza


# Método que calcula e retorna a Entropia
def entropy(pi, n):
	count = 0
	if n==0:
		return count
	for p in pi:	
		x = -p/n
		if (x != 0):
			x *= math.log2(p/n)
		count += x
	return count


# Função que calcula a AverageInformation dos dados
def averageInformation(arrayi, arraySum, n, criterion, prune):
	count = 0
	countaux = 0
	if n==0:
		return 0
	for i in range(len(arrayi[0])):
		arrayAux = []
		for j in range(len(arrayi)):
			arrayAux.append(arrayi[j][i])
			countaux += arrayi[j][i]
		countaux /= n
		if criterion == 'entropy':
			countaux *= entropy(arrayAux, arraySum[i])
		elif criterion == 'gini':
			countaux *= gini(arrayAux, arraySum[i])
		else:
			countaux *= errorReason(arrayAux, arraySum[i])
		count += countaux
	return count


# Função que retorna o "ganho"
def gain(array, n, arrayi, arraySum, criterion, prune):
	if criterion == 'entropy':
		return entropy(array, n) - averageInformation(arrayi, arraySum, n, criterion, prune)
	elif criterion == 'gini':
		return gini(array, n) - averageInformation(arrayi, arraySum, n, criterion, prune)
	else:
		return errorReason(array, n) - averageInformation(arrayi, arraySum, n, criterion, prune)


# Método que conta e retorna a quantidade total de valores que encontram-se na posição arrayInfo[i][0]
def countNtotal(arrayInfo):
	count = 0
	arraycount = []
	for i in arrayInfo:
		arraycount.append(i[0])
		count += i[0]
	return arraycount, count


# Método que retorna a soma na posição y (em que [x][y]) de uma matrix em array
def sumOfMatrix(arrayi):
	arraySum = []
	for i in range(len(arrayi[0])):
		count = 0
		for j in range(len(arrayi)):
			count += arrayi[j][i]
		arraySum.append(count)
	return arraySum


# Método que calcula e retorna o index da posição com melhor "ganho"
def chooseGain(arrayInfo, criterion, prune):
	maxvalue = 0
	indexofmaxvalue = 0
	arrayNtotal, Ntotal = countNtotal(arrayInfo)
	for i in range(len(arrayInfo[0][1])):
		arrayi = []
		for j in range(len(arrayInfo)):
			arrayi.append(arrayInfo[j][1][i])
		arraySum = sumOfMatrix(arrayi)
		x = gain(arrayNtotal, Ntotal, arrayi, arraySum, criterion, prune)
		if x > maxvalue:
			maxvalue = x
			indexofmaxvalue = i
	return indexofmaxvalue


# Método que converte e retorna data (x e y) em lista
def converter(x, y):
	arquivo = dict()
	for m in range(len(x)):
		for n in range(len(y)):
			if m == 0:
				arquivo.setdefault(y[n], [])
			if not x[m][n] in arquivo[y[n]]:
				arquivo[y[n]].append(x[m][n])
	return arquivo


# Método que retorna a posição da variável no array
def foundPositionInArray(x, array):
	pos = -1
	for i in range(len(array)):
		if array[i] == x:
			pos = i
			break
	return pos


# Método que calcula e retorna o tamanho máximo de cada chave da lista pode ter exceto a ultima posição
def findMaxSize(lista):
	maxSize = 0
	j = len(lista)
	for i in lista:
		j -= 1
		if j <= 0:
			break
		if len(lista[i]) > maxSize:
			maxSize = len(lista[i])
	return maxSize


# Método que converte a data (x, y) para um array do tipo: arrayInfo[m][n], em que m varia o tipo de variável,
# e que em n: a 1º posição é o total da quantidade e na 2º é essa quantidade distribuidade pelas variáveis
def countInMatrix(x, y):
	countVariablesWithoutFinal = len(y)-1
	arrayInfo = []
	lista = converter(x, y)
	maxNumberOfEachVariable = findMaxSize(lista)
	numberOfResults = len(lista[y[countVariablesWithoutFinal]])
	for i in range(numberOfResults):
		arrayInfo.append([0, np.zeros([countVariablesWithoutFinal, maxNumberOfEachVariable], dtype = int)])
	for m in range(len(x)):
		positionResult = foundPositionInArray(x[m][countVariablesWithoutFinal], lista[y[countVariablesWithoutFinal]])
		arrayInfo[positionResult][0]+=1
		for n in range(countVariablesWithoutFinal):
			positionvar = foundPositionInArray(x[m][n], lista[y[n]])
			arrayInfo[positionResult][1][n][positionvar]+=1
	return arrayInfo
		

# Método que retorna a variável com maior "ganho" e a sua respetiva posição
def id3(x, y, arrayInfo, criterion, prune):
	x = chooseGain(arrayInfo, criterion, prune)
	return y[x], x


# Método que retorna a informação da data do respetivo filename
def openData(FILENAME):
	with open (FILENAME, newline='') as f:
		reader = csv.reader(f)
		w = list(reader)
	
	data = np.array(w)
	
	argument = data[0]
	x = data[1:, 0:-1]
	y = data[1:, -1]

	return x, y, argument


# Método que verifica se as restantes escolhas têm todas o mesmo resultado
def checkNodeResult(x, y):
	lastposition = len(y)-1
	var = x[0][lastposition]
	for i in range(len(x)):
		if x[i][lastposition] != var:
			return True
	return False


# Método que retira toda a informção que não tenha a informação desejada
def onlydatawithnode(x, y, node, nodeposition):
	size = len(x)
	i = 0
	while i < size:
		if x[i][nodeposition] != node:
			newx = np.delete(x, i, 0)
			x = newx
			i-=1
			size-=1
		i+=1
	
	
	newx = np.delete(x, nodeposition, 1)
	x = newx

	newy = np.delete(y, nodeposition, 0)
	y = newy

	return x, y


# Método que verifica se a variável encontra-se no array
def checkinarray(array, var):
	for i in array:
		if i == var:
			return True
	return False


# Método que retorna uma lista com os resultados possíveis com a sua quantidade
def listResults(matrix, positionLista):
	lista = dict()
	for i in range(len(matrix)):
		lista.setdefault(positionLista[i], [])
		lista[positionLista[i]].append(matrix[i][0])
	return lista


# Método que retorna o Index com maior quantidade no arrayInfo
def findBigger(arrayInfo):
	bigger = 0
	for i in range(len(arrayInfo)):
		if arrayInfo[i][0]>bigger:
			bigger = i
	return bigger


# Método que retorna a variável (nome) com mais quantidade na posição list[i][0]
def foundMaxInList(list):
	max = 0
	maxIndex = ""
	for i in list:
		if list[i][0] > max:
			max = list[i][0]
			maxIndex = i
			break
	return maxIndex


# Método recursivo auxiliar para fazer o método pruning
def pruningRecursive(root, x, y, rootOriginal):

	leafs = True
	for i in root.child:
		if i.child != []:
			leafs = False
	if leafs == False:
		for j in root.child:
			if j.child != []:
				if pruningRecursive(j, x, y, rootOriginal):
					
					return True
		return False
	else:
		beforePrune = accuracy(rootOriginal, x, y)

		saveChilds = deepcopy(root.child)
		savekey = root.key

		root.child = []
		root.key = foundMaxInList(root.results)

		afterPrune = accuracy(rootOriginal, x, y)
		if beforePrune < afterPrune:
			return True
		
		root.child = deepcopy(saveChilds)
		root.key = savekey
		
	return False


# Método que executa o pruning na árvore
def pruning(root, x, y):
	while(True):
		if not pruningRecursive(root, x, y, root):
			break


# Método que retorna a exatidão de uma matrix
def exatidao(matrix):
	total = 0
	diagonal = 0
	for i in range(len(matrix)):
		for j in range(len(matrix)):
			total += matrix[i][j]
			if i == j:
				diagonal += matrix[i][j]
	return (diagonal)/total


# Função recursiva referente á função makeTree
def makeTreeRecursive(x, y, root, fathernode, fathernodeposition, criterion, prune):
	lista = converter(x, y)
	size = lista[fathernode]
	
	for i in range(len(size)):
		newx = deepcopy(x)
		newy = deepcopy(y)
		newx, newy = onlydatawithnode(newx, newy, size[i], fathernodeposition)
		arrayInfo = countInMatrix(newx, newy)
		newlista = converter(newx, newy)
		if checkNodeResult(newx, newy):
			if len(newy) > 1:
				node, nodeposition = id3(newx, newy, arrayInfo, criterion, prune)
				root.child.append(Node(node, fathernode, listResults(arrayInfo, newlista[newy[len(newy)-1]]), size[i]))
				makeTreeRecursive(newx, newy, root.child[i], node, nodeposition, criterion, prune)
			else:
				root.child.append(Node(newx[findBigger(arrayInfo)][len(newx[0])-1], fathernode, listResults(arrayInfo, newlista[newy[len(newy)-1]]), size[i]))
		else:
			root.child.append(Node(newx[0][len(newy)-1], fathernode, listResults(arrayInfo, newlista[newy[len(newy)-1]]), size[i]))


# Método que cria uma árvore a partir das decisões e dos dados escolhidos
def makeTree(x, y, criterion, prune):
	lista = converter(x, y)
	arrayInfo = countInMatrix(x, y)
	node, nodeposition = id3(x, y, arrayInfo, criterion, prune) 
	root = Node(node, "none", listResults(arrayInfo, lista[y[len(y)-1]]), "none")
	ROOT = root
	makeTreeRecursive(x, y, root, node, nodeposition,  criterion, prune)
	return ROOT


# Método que calcula a exatidão a partir da raiz da árvore e dos elementos x e y
def accuracy(ROOT, x, y):
		lista = converter(x, y)
		results = lista[y[len(y)-1]]
		sizeFinalMatrix = len(results)
		finalMatrix = np.zeros([sizeFinalMatrix, sizeFinalMatrix], dtype = int)
		
		for i in range(len(x)):
			trueResult = x[i][len(x[i])-1]
			root = ROOT

			while(True):
				find = False
				if root.child == []:
					testResult = root.key
					break
				else:
					position = foundPositionInArray(root.key, y)
					for j in root.child:
						if j.path == x[i][position]:
							root = j
							find = True
					if find == False:
						index = foundMaxInList(root.results)
						testResult = index
						break
			
			finalMatrix[foundPositionInArray(trueResult, results)][foundPositionInArray(testResult, results)] += 1
		
		return exatidao(finalMatrix)
