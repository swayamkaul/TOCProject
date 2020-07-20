from rishabh.NN import perceptron
import numpy as np
print("Enter X value")


f=open('../percentage.txt')
k=float(f.read())
x_in=k


print("Enter Y value")

# y will come from fire base
###############
y_in=int(input())
###############


X = np.array([[x_in]])
y = np.array([[y_in]])

print('Testing....')
print('Loading Weights.....')
weights=np.load('weights.npy')
p = perceptron(X.shape[1])

new_weights=p.update(X,y,weights)
np.save('weights.npy',new_weights)
print('new weights saved....')
print('Results')
print('Loading Weights...')
weights=np.load('weights.npy')

for x in X:
    pred = p.predict(x,weights)
    print('[INFO]: Data={},  Prediction={}'.format(x, pred))

