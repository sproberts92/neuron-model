# Neuron Model

A simulation of a neuronal network for the purpose of studying data storage within the network.

Written as part of a thesis project for the degree of Master of Science in Theoretical Physics at Utrecht University. Thesis title ["A neuron based model for data storage and retrieval"](https://dspace.library.uu.nl/handle/1874/335288). The project was interdisciplinary in nature and approched the problem from a statistical perspective.

## Usage

Build from the command line with
```
nmake /f NMAKEFILE
```

The Linux `MAKEFILE` has not been kept up to date as I work primarily from Windows. I do intend to bring it up to date soon.

Requires libconfig to read configuration from file (this will be replaced by YAJL or similar in the future).

Requires the library `sproberts92/engine-opengl` for real time visualisation, however this is not yet public. It needs a good clean out, but should be available soon. In the mean time the progam can still be used to gather data.

Requires catch for testing, although due to time constraints while writing the thesis there are very few tests written. I may add more later but as the tests are built separately from the main program catch is not required for building the main program.

There are a number of Python visualisation scripts in the `visualisation` directory. Most require NumPy and Matplotlib.
