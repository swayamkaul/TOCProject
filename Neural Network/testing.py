from rishabh.NN import perceptron
import numpy as np

open('out.txt','w')as f:
    k=f.read()

X = np.array([[k]])

print('Testing....')
print('Loading Weights.....')
weights=np.load('weights.npy')
p = perceptron(X.shape[1])

for x in X:
    pred = p.predict(x,weights)
    print('[INFO]: Data={},  Prediction={}'.format(x, pred))
