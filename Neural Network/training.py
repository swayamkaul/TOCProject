  
from rishabh.NN import perceptron
import numpy as np

#X = np.array([[30], [40], [50], [60],[70],[45],[49]])
#y = np.array([[0], [0], [1], [1],[1],[1],[0],[0]])
X = np.arange(100).reshape(100,1)
y=[]
for i in range(100):
    if i <=50:
        y.append(0)
    else:
        y.append(1)

y=np.array(y).reshape(100,1)


print('Training....')
p = perceptron(X.shape[1])
weights=p.fit(X, y, epochs=150)
print('Done')
print(weights)
np.save('weights.npy',weights)
print('weights saved....')
print('Results')
print('Loading Weights...')
weights=np.load('weights.npy')
for x in X:
    pred = p.predict(x,weights)
    print('[INFO]: Data={},  Prediction={}'.format(x, pred))