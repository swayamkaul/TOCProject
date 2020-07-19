import numpy as np 
class perceptron:
    def __init__(self, N, a=0.1):
        self.W = np.random.randn(N + 1) / np.sqrt(N)
        self.a = a

    def step(self, x):
        return 1 if x > 0 else 0

    def fit(self, X, y, epochs=50):
        X = np.c_[X, np.ones((X.shape[0]))]

        for epoch in np.arange(0, epochs):
            for (x, target) in zip(X, y):
                pred = self.step(np.dot(x, self.W))

                if pred != target:
                    error = pred - target

                    self.W += -self.a * error * x
        return self.W


    def update(self, X, y,weights ):
        X = np.c_[X, np.ones((X.shape[0]))]

        for epoch in np.arange(0, 1):
            for (x, target) in zip(X, y):
                pred = self.step(np.dot(x, weights))

                if pred != target:
                    error = pred - target

                    weights += -self.a * error * x
        return weights


    def predict(self, X,weights,add_bias=True):
        X = np.atleast_2d(X)

        if add_bias:
            X = np.c_[X, np.ones((X.shape[0]))]

        return self.step(np.dot(X, weights))