from ExtraFunctions import *
from sklearn.model_selection import train_test_split


# Variáveis Globais
FILENAME = 'weather.nominal.csv'	# 'weather.nominal.csv', 'contact-lenses.csv', 'vote.csv', 'soybean.csv'
CRITERION = 'entropy'	# 'entropy', 'gini', 'error'
PRUNE = False			# True, False


class DecisionTreeREPrune:
	ROOT = None

	# Cria uma instância da árvore com decisões
	def __init__(self, criterion, prune):
		self.criterion = criterion
		self.prune = prune


	# Treina os dados e cria uma ávore
	def fit(self, x, y):
		if self.prune:
			x_tree, x_prune, y_tree, y_prune = train_test_split(x, y, random_state=0)

			newy = y_tree.reshape(len(y_tree), 1)
			tree = np.append(x_tree, newy, axis=1)

			self.ROOT = makeTree(tree, ARGUMENT, self.criterion, self.prune)

			# Se necessário imprimir a árvore antes do prunning
			#Node.printNodeLevelWise(self.ROOT)
			#print("..................................")
			#print("..................................")
			#print("..................................")

			newy = y_prune.reshape(len(y_prune), 1)
			prune = np.append(x_prune, newy, axis=1)

			pruning(self.ROOT, prune, ARGUMENT)

			# Se necessário imprimir a árvore depois do prunning
			#Node.printNodeLevelWise(self.ROOT)
		else:
			newy = y.reshape(len(y), 1)
			data = np.append(x, newy, axis=1)

			self.ROOT = makeTree(data, ARGUMENT, self.criterion, self.prune)
			
			#Se necessário imprimir a árvore
			#Node.printNodeLevelWise(self.ROOT)


	# Retorna a Exatidão da árvore
	def score(self, x, y):
		newy = y.reshape(len(y), 1)
		data = np.append(x, newy, axis=1)
		return accuracy(self.ROOT, data, ARGUMENT)
	

# Função main
def main():

	global ARGUMENT
	x, y, ARGUMENT = openData(FILENAME)

	x_train, x_test, y_train, y_test = train_test_split(x, y, random_state=0)

	classifier = DecisionTreeREPrune(CRITERION, PRUNE)
	classifier.fit(x_train, y_train)
	result = classifier.score(x_test, y_test)
	
	print("Percentagem de casos corretamente classificados {:2.2%}".format(result))
	

if __name__ == "__main__":
    main()