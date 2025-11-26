#sudo apt-get install doxygen
#sudo apt-get install curl
#curl -fsSL https://miktex.org/download/key | sudo tee /usr/share/keyrings/miktex-keyring.asc > /dev/null
#Ubuntu 20.04 LTS (Focal Fossa):
#echo "deb [signed-by=/usr/share/keyrings/miktex-keyring.asc] https://miktex.org/download/ubuntu focal universe" | sudo tee /etc/apt/sources.list.d/miktex.list
#sudo apt-get install miktex

doxygen

cd latex
pdflatex refman.tex