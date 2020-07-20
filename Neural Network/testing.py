from rishabh.NN import perceptron
import numpy as np

f=open('../percentage.txt')
k=float(f.read())
print(k)


X = np.array([[k]])

print('Testing....')
print('Loading Weights.....')
weights=np.load('weights.npy')
p = perceptron(X.shape[1])

for x in X:
    pred = p.predict(x,weights) # output to firebase as result
    print('[INFO]: Data={},  Prediction={}'.format(x, pred))
    
    
