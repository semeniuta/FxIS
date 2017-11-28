class Dummy:

    def __init__(self):
        pass

    def __enter__(self):
        print("Enter")
        return self

    def __exit__(self, type, value, traceback):
        print("Exit")

    def hello(self):
        print("Hello")


if __name__ == '__main__':
    
    with Dummy() as d:
        print(d)
        d.hello()