from rishabh.NN import perceptron
import numpy as np

X = np.arange(100).reshape(100,1)

print('Testing....')
print('Loading Weights.....')
weights=np.load('weights.npy')
p = perceptron(X.shape[1])

for x in X:
    pred = p.predict(x,weights)
    print('[INFO]: Data={},  Prediction={}'.format(x, pred))