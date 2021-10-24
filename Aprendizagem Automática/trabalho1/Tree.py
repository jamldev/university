class Node :

	# Cria uma instância do nó com argumentos
	def __init__(self, key, father, results, path): 
		self.key = key  
		self.child = []
		self.father = father
		self.results = results
		self.path = path
	
	# Imprime a árvore a partir do nó escolhido
	def printNodeLevelWise(root):
		if root is None: 
			return
		queue = [] 
		queue.append(root)
		while(len(queue)>0):
			n = len(queue)
			while(n > 0) :
				p = queue[0]
				queue.pop(0)
				print("key = %s | father = %s | results = %s | path = %s" %(p.key, p.father, p.results, p.path))
				for index, value in enumerate(p.child):
					queue.append(value)
				n -= 1
			print("----------------------------")
