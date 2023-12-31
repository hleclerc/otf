# sudo docker build --rm --tag otf .
# sudo docker run -it --rm -p 8888:8888 otf

FROM quay.io/jupyter/base-notebook

# libcgal-dev git g++ 
USER root
RUN apt-get update \
 && apt-get install -yq \
    libgl1-mesa-glx \
    libxrender-dev \
    libgl1 \
    xvfb \
    libcgal-dev \
    git \
    g++ \
 && apt-get clean

# pyind11 (ipywidgets jupyter-server-proxy)
USER ${NB_UID}
RUN conda install -c conda-forge pybind11 eigen && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "/home/${NB_USER}"

# pyind11
USER root
RUN pip install --no-cache-dir 'pyvista[all,trame]' jupyter-server-proxy && \
    fix-permissions "${CONDA_DIR}" && \
    fix-permissions "/home/${NB_USER}"

# git + build otf
USER ${NB_UID}
RUN git clone https://github.com/hleclerc/otf.git
RUN cd otf/wrappers/python && python setup.py build 

# install otf
USER root
RUN cd otf/wrappers/python && python setup.py install

USER ${NB_UID}

EXPOSE 8888
