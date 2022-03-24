#  Manual de implementación de un esquema de computación distribuida para Geant4 

## Requisitos

- Distribución Linux basada en debian (32/64 bits)
- Open MPI (4.0.4) https://www.open-mpi.org/software/ompi/v4.0/
- Servidor NFS
- Opcional: ROOT for histogramming/analysis

## Pasos para la implementación

**Configurar máquias**
- Configuración de usuarios de cada máquina
- Configurar ip estática
- Editar archivos hosts
- Configurar ssh

**Instalación de Geant4**
- Instalando pre requisitos
- Instalando Geant4
- Ejecutar Ejemplo B1

**Compilando G4mpi**
- Compilación G4mpi
- Ejecutando ejemplo exMP01

**Configurando servidor NFS**
- Configuración servidor en el maestro
- Configuración en los esclavos
- Prueba NFS

**Instalación Open-mpi**
- Configuración
- Instalación
- Ejecutando ejemplos

**Ejecutando ejemplo en el sistema distribuido**
- Configurando variables de entorno
- Compilando ejemplo exMPI01
- Ejecutando ejemplo exMPI01

**Configurar una aplicación para uso con MPI**
- Configuración sesión con G4mpi
- Configuración CMakeLists.txt
- Configuración Clúster
- Comando Ejecución con Open mpi


## Configurar máquinas 

### Configuración de usuarios de cada máquina

Todas las máquinas se configuran con el mismo nombre de usuario, así en cada máquina 
abrimos una terminal y ejecutamos los siguientes comandos:

Entramos al root:

```sh
$ sudo su 
```
Agregamos el usuario:

```sh
$ adduser gfnun
```
Se solicitara el ingreso de la clave y unos datos adicionales los cuales deberan 
ingresarse si se desea, de lo contrario solo hacer enter y continuar.

Comprobar que se creo el usuario:

```sh
$ cd ~
$ ls /home
```
se debera ver la carpeta del usuario, adicional ingresamos para comprobar:

```sh
$ ssh gfnun@0.0.0.0
```
salimos:

```sh
$ exit
```

Ahora le daremos permisos de administrador, desde el root:

```sh
$ adduser gfnun sudo
```

### Configurar ip estática

Comprobar las interfaces de red que tiene el equipo.

```sh
$ ifconfig -a
```
Edita el archivo de configuración de las interfaces de red con el siguiente comando:

```sh
$ sudo nano /etc/network/interfaces
```

En este caso se va configurar la interfaz eth0,

```
# Configuración de dirección IP fija para el interfaz eth0
auto eth0
iface eth0 inet static
address 192.168.1.50
netmask 255.255.255.0
network 192.168.1.0
broadcast 192.168.1.255
gateway 192.168.1.1
```
Reiniciar las interfaces de red del equipo:

```sh
$ sudo /etc/init.d/networking restart
```
Alternativamente:
```sh
$ sudo ifconfig eth0 down 
$ sudo ifconfig eth0 up
```
### Editar archivo hosts

Editar el archivo hosts del maestro para colocarle nombre a cada máquina:

```sh
sudo nano /etc/hosts
```

A continuación se aprecia como debería quedar el archivo hosts:

```
127.0.0.1       localhost
127.0.1.1       MrHerbert.gfnun.unal.edu.co     MrHerbert

#MPI Cluster Setup
192.168.9.144 master
192.168.9.105 slave1
192.168.9.109 slave2


# The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters

```

### Configurar ssh

Lo siguiente es configurar los accesos a través de ssh entre las máquinas para que no
solicite clave cada vez que la máquina maestro (master) acceda a cada esclavo (slave1, slave2..etc)

En el *master* generar la llave rsa.pub

```sh
$ ssh-keygen -t rsa
```
Presionar enter 3 veces, no escribir nada.


Verificar si se tiene el archivo authorized_keys, hacer esto para cada máquina (maestro y esclavos)
```sh
$ ls -l .ssh/
```
Si no se encuentra, crearlo:

```sh
$ touch ~/.ssh/authorized_keys
```
Generar permisos al archivo:

```sh
$ chmod 600 ~/.ssh/authorized_keys
```

Copiar llaves generadas desde el master a los esclavos:

```sh
$ scp .ssh/id_rsa.pub gfnun@slave1:.ssh/authorized_keys
$ scp .ssh/id_rsa.pub gfnun@slave2:.ssh/authorized_keys
```
También se debe copiar en el mismo master para que no solicite clave al hacer ssh sobre el mismo:

```sh
$ cp ~/.ssh/id_rsa.pub ~/.ssh/authorized_keys
```
Esto es importante ya que con la implementación de mpi a veces se desea usar el mismo master
para ejecutar aplicaciones.

Así no pedirá contraseña cada vez que se va ingresar a cada nodo desde el master,
bastara escribir algo así:

```sh
$ ssh slave1
```

## Instalación Geant4

### Instalación pre requisitos

- **CLHEP** http://proj-clhep.web.cern.ch/proj-clhep/clhep23.html
leer el archivo README.txt, luego crear una carpeta Build y una install dentro de la carpeta build hacer:

```sh
$ cmake -DCMAKE_INSTALL_PREFIX=<install_dir> <source_code_dir>
$ cmake --build . --config RelWithDebInfo
$ cmake --build . --target install
```

- **Zlib**
```sh
$ sudo apt install zlib1g-dev
```

- **QT**
```sh
$ sudo apt-get install libqt4-dev
```


- **OpenGL**
```sh
$ sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```


- **ray tracer**
```sh
$ sudo apt install povray
```

- **X11 libraries**
```sh
$ sudo apt-get install xorg-dev
```

- **g++ y gcc**
```sh
$ sudo apt-get update -y
$ sudo apt-get install -y wget g++-5 gcc-5 cmake libexpat1-dev vim cmake-curses-gui freeglut3 freeglut3-dev mesa-utils python libx11-dev libxmu-dev expat 
$ sudo apt-get install -y libfontconfig1-dev libfreetype6-dev libxcursor-dev libxext-dev libxfixes-dev libxft-dev libxi-dev libxrandr-dev libxrender-dev  
$ sudo apt-get install libssl-dev
```
- **Configurando g++ y gcc**
```sh
$ sudo rm /usr/bin/gcc 
$ sudo rm /usr/bin/g++ 
$ sudo ln -s /usr/bin/gcc-5 /usr/bin/gcc 
$ sudo ln -s /usr/bin/g++-5 /usr/bin/g++ 
```
### Instalación de Geant4
Creación carpetas de instalación:
```sh
$ cd $HOME
$ sudo mkdir -p $HOME/Geant4-10.6/src 
$ sudo mkdir -p $HOME/Geant4-10.6/build 
$ sudo mkdir -p $HOME/Geant4-10.6/install 
$ sudo mkdir -p $HOME/Geant4-10.6/data 
```
Descarga:

```sh
$ G4_VERSION="10.06.p01"
$ QT_VERSION="4.8.7"
$ sudo wget http://cern.ch/geant4-data/releases/geant4.${G4_VERSION}.tar.gz
$ sudo tar xf geant4.${G4_VERSION}.tar.gz -C $HOME/Geant4-10.6/src
$ sudo rm geant4.${G4_VERSION}.tar.gz 
```
Compilación:
```sh
$ cd $HOME/Geant4-10.6/build
$ cmake -DCMAKE_INSTALL_PREFIX=$HOME/Geant4-10.6/install \
      -DGEANT4_USE_OPENGL_X11=ON \
      -DQT_QMAKE_EXECUTABLE=/usr/local/Trolltech/Qt-${QT_VERSION}/bin/qmake \
      -DGEANT4_USE_QT=OFF \
      -DGEANT4_INSTALL_DATA=ON \
      -DGEANT4_INSTALL_DATADIR=$HOME/Geant4-10.6/data \
      -DGEANT4_BUILD_MULTITHREADED=ON \
      -DGEANT4_INSTALL_EXAMPLES=ON \
      ../src/geant4.${G4_VERSION}
      
$ make -j`nproc`      
```
Instalación:
```sh
$ sudo make install 
```

Varables de entorno:
```sh
$ echo "export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/Trolltech/Qt-${QT_VERSION}/lib" >> $HOME/.bashrc
$ echo "source $HOME/Geant4-10.6/install/bin/geant4.sh" >> $HOME/.bashrc
```

Verificar instalación y configuración:
```sh
$ ./geant4-config --help
```
### Configurar variables de entorno para Geant4

Abrir el archivo bashrc:

```sh
$ sudo nano ~/.bashrc
```
Añadir al final las siguiente líneas:

```sh
export G4ABLADATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4ABLA3.1
export G4ENSDFSTATEDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4ENSDFSTATE2.2
export G4INCLDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4INCL1.0
export G4LEDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4EMLOW7.9.1
export G4LEVELGAMMADATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/PhotonEvaporation5.5
export G4NEUTRONHPDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4NDL4.6
export G4PARTICLEXSDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4PARTICLEXS2.1
export G4PIIDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4PII1.3
export G4REALSURFACEDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/RealSurface2.1.1
export G4SAIDXSDATA=/home/gfnun/Geant4-10.6/install/share/Geant4-10.6.1/data/G4SAIDDATA2.0
```

### Ejecutar ejemplo B1:

```sh
$ cd /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/basic/B1
$ mkdir build-B1 
```
```sh
$ cmake –DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/basic/B1
$ make -j4
$ ./exampleB1
```

## Compilando G4mpi

### Compilación G4mpi:

```sh
$ geant4
$ mkdir build-mpi
$ cd build-mpi
$ cmake -DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 \
     -DCMAKE_INSTALL_PREFIX=/home/gfnun/Geant4-10.6/geant4-mpi \
     /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/extended/parallel/MPI/source
$ make
$ make install
```

### Ejecutando ejemplo exMPI01:

Compilando:
```sh
$ mkdir build
$ cd build
$ sudo cmake -DG4mpi_DIR=/home/gfnun/Geant4-10.6/geant4-mpi/lib/G4mpi-10.6.1 -DCMAKE_CXX_COMPILER=mpicxx \
      -DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/extended/parallel/MPI/examples/exMPI01
$ make
$ make install
```
Ejecutando, asegurarse de que este en la carpeta build:

```sh
mpiexec ./exMPI01
```
El mpiexec con el que se ejecuta es desde el open mpi que viene por defecto en la distribución:

```sh
mpiexec --help
```

## Configurando servidor NFS

En el MASTER:

```sh
$ sudo apt install nfs-kernel-server portmap
$ mkdir sharedFolder
$ ls sharedFolder/ 
$ sudo chown nobody:nogroup sharedFolder/
```
Ver el estado del servidor:

```sh
$ sudo /etc/init.d/nfs-kernel-server status
```

Detener el servicio:

```sh
$ sudo /etc/init.d/nfs-kernel-server stop
```

Modificar el archivo exports

```sh
$ sudo pico /etc/exports
```
Añadir:

```sh
/home/gfnun/sharedFolder/ *(rw,sync,no_subtree_check)
/home/gfnun/Geant4-10.6/ *(rw,sync,no_subtree_check)
/home/gfnun/Geant4-10.5/ *(rw,sync,no_subtree_check)
```
Volver a la terminal, y hacer que todos los directorios colocados allí se exporten:

```sh
$ sudo exportfs -a
```

Verificar:

```sh
$ sudo exportfs
```
Salida en la terminal:

```sh
/home/gfnun/sharedFolder
		<world>
/home/gfnun/Geant4-10.6
		<world>
/home/gfnun/Geant4-10.5
		<world>
```

En los esclavos:

```sh
$ sudo apt-get install nfs-common portmap
$ mkdir -p sharedFolder/
$ ls sharedFolder
$ mkdir -p Geant4-10.6/
$ ls sharedFolder
```

En el maestro iniciar el servidor:

Ver el estado:

```sh
$ sudo /etc/init.d/nfs-kernel-server status
```

iniciar servicio:

```sh
$ sudo /etc/init.d/nfs-kernel-server start
```

En cada Esclavo:

```sh
$ sudo mount master:/home/gfnun/sharedFolder sharedFolder/
$ sudo mount master:/home/gfnun/Geant4-10.6/ Geant4-10.6/
$ sudo mount master:/home/gfnun/Geant4-10.5/ Geant4-10.5/
```
Verificar que se esta compartiendo:

```sh
$ df -h
```

La salida debería ser:

```sh
master:/home/gfnun/sharedFolder               723G  440G  247G  65% /home/gfnun/sharedFolder
master:/home/gfnun/Geant4-10.6                723G  440G  247G  65% /home/gfnun/Geant4-10.6
master:/home/gfnun/Geant4-10.5                723G  440G  247G  65% /home/gfnun/Geant4-10.5
```

En uno de los esclavos podemos probar que se comparten los archivos:

```sh
$ cd sharedFolder/
$ touch test.txt
```
y verificar que se vea desde el maestro.


## Instalación Open mpi 

Descargar de la página https://www.open-mpi.org/software/ompi/v4.0/ el archivo openmpi-4.0.3.tar.gz
dejarlo en el directorio:

```sh
$ pwd
/home/gfnun/
```
Descomprimir en la carpeta sharedFolder/ :

```sh
$ cp openmpi-4.0.3.tar.gz sharedFolder/
$ cd sharedFolder/
$ mkdir openmpi-install/
$ tar -xzf openmpi-4.0.3.tar.gz
```

Compilar:

```sh
$ cd openmpi-4.0.3/
$ mkdir build
$ cd build
$ ../configure --prefix=/home/gfnun/sharedFolder/openmpi-install/ --enable-mpi-cxx --enable-orterun-prefix-by-default 
$ sudo make -j4
```

Instalar:

```sh
$ sudo make install
$ sudo ldconfig
```
Para usar open mpi instalado en la carpeta sharedFolder/ cargamos las variables de entorno:

```sh
$ export PATH=/home/gfnun/sharedFolder/openmpi-install/bin:$PATH
$ export LD_LIBRARY_PATH=/home/gfnun/sharedFolder/openmpi-install/lib:$LD_LIBRARY_PATH 
```

Si se desea se puede dejar en el bashrc:

```sh
echo "export PATH=/home/gfnun/sharedFolder/openmpi-install/bin:$PATH" >> $HOME/.bashrc
echo "export LD_LIBRARY_PATH=/home/gfnun/sharedFolder/openmpi-install/lib:$LD_LIBRARY_PATH" >> $HOME/.bashrc
```


Archivo hostfile: En este archivo se indica los procesos a usar por máquina. 

```sh
$ nano my_hosts
```
Añadir:

```sh
slave1 slots=2
slave2 slots=2
```



Ejecutar ejemplo:

```sh
$ cd ..
$ cd examples
$ sudo mpicc hello_c.c -o hello_c
$ mpiexec -np 4 --hostfile my_hosts ./hello_c
```

## Ejecutando ejemplos de geant4 en el sistema distribuido

### Ejemplo exMPI01

Compilar:

```sh
$ mkdir build
$ cd build
$ sudo cmake -DG4mpi_DIR=/home/gfnun/Geant4-10.6/geant4-mpi/lib/G4mpi-10.6.1 -DCMAKE_CXX_COMPILER=mpicxx \
      -DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/extended/parallel/MPI/examples/exMPI01
$ sudo make
$ sudo make install

```
Ejecutar:

```sh
$ cd build
$ mpiexec -np 4 ./exMPI01
```



### Ejemplo exMPI04

Compilar, para compilar se debe hacer desde el compilador de open mpi instalado en la carpeta sharedFolder/

```sh
$ mkdir build
$ cd build
$ sudo cmake -DG4mpi_DIR=/home/gfnun/Geant4-10.6/geant4-mpi/lib/G4mpi-10.6.1 -DCMAKE_CXX_COMPILER=/home/gfnun/sharedFolder/openmpi-install/bin/mpicxx \
      -DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 /home/gfnun/Geant4-10.6/src/geant4.10.06.p01/examples/extended/parallel/MPI/examples/exMPI04
$ make
$ make install
```

Ejecutar con Open mpi:

Exportar variables, en este caso la instalación de open mpi se realizó en sharedFolder:

```sh
export PATH=/home/gfnun/sharedFolder/openmpi-install/bin:$PATH
export LD_LIBRARY_PATH=/home/gfnun/sharedFolder/openmpi-install/lib:$LD_LIBRARY_PATH 
```
De esta manera se ejecuta en la máquina local:

```sh
$ cd build
$ mpiexec -n 4 ./exMPI04
```

## Configurar una aplicación para uso con MPI

Para este ejemplo se va usar la aplicación g4pntest, ubicada en este repositorio en la carpeta 00_g4_apps/


### Configuración sesión con G4mpi

Agregar las librerias necesarias para el uso de G4mpi, en PNtest.cc:

```c++
#include "G4MPImanager.hh"
#include "G4MPIsession.hh"
```

Luego en la función main() del archivo PNtest.cc se añade las siguientes líneas de código, para este ejemplo se añade después de definir la semilla:

```c++
  // --------------------------------------------------------------------
  // MPI session
  // --------------------------------------------------------------------
  G4MPImanager* g4MPI = new G4MPImanager(argc, argv);
  // MPI session (G4MPIsession) instead of G4UIterminal
  // Terminal availability depends on your MPI implementation.
  G4MPIsession* session = g4MPI-> GetMPIsession();
  G4String prompt = "[40;01;33m";
  prompt += "G4MPI";
  prompt += "[40;31m(%s)[40;36m[%/][00;30m:";
  session-> SetPrompt(prompt);
```
En este caso la aplicación tiene activado Multithreading (MT):

```c++
#ifdef G4MULTITHREADED //Modified by Javier
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager-> SetNumberOfThreads(4);
#else
  G4RunManager* runManager = new G4RunManager;
#endif
```
Se deja sin MT, así:

```c++
//#ifdef G4MULTITHREADED //Modified vy Javier
//  G4MTRunManager* runManager = new G4MTRunManager;
//  runManager-> SetNumberOfThreads(4);
//#else
  G4RunManager* runManager = new G4RunManager;
//#endif
```

Para la sesión se comentan o se quitan las siguientes líneas de código:

```c++
/* ORIGINAL
  // get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();  

  if(argc == 1)
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UI->ApplyCommand("/control/execute vis.mac");
#endif
      ui->SessionStart();
      delete ui;
#endif
    }
  // Batch mode
  else
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }
*/
```
Se inicia la sesión:

```c++
  session-> SessionStart();
```

Por último se elimina la sesión de mpi agregando la siguiente línea de código antes del return 0:

```c++
  delete g4MPI;
  delete runManager;
  return 0;
```

### Configuración CMakeLists.txt

Para el paquete de G4mpi agregar:

```c++
find_package(G4mpi REQUIRED) 
```
Para incluir los directorios, se deja así:

```c++
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
			      ${Geant4_INCLUDE_DIR}
			      ${G4mpi_INCLUDE_DIR})
```

Añadir ejecutable y enlace con las librerias Geant4:

```c++
target_link_libraries(PNtest ${G4mpi_LIBRARIES})
```
### Configuración archivos .csv

La función RunAction::BeginOfRunAction(const G4Run* aRun), se deja así, para la generación de cada archivo .csv en cada nodo o cada hilo:

```c++
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // ntuples will be written on each rank
  G4int rank = G4MPImanager::GetManager()->GetRank();
  std::ostringstream fname;
  fname<<"out_test-rank"<<rank;


  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(2);
  analysisManager->SetFileName(fname.str());

  analysisManager->OpenFile();
  analysisManager->CreateH1("Edep","Energy (keV)", 1000, 0., 1000.); // Id = 0
}
```

Por último la función void RunAction::EndOfRunAction(const G4Run* aRun) se deja así:

```c++
void RunAction::EndOfRunAction(const G4Run* aRun)
{
  //if(!IsMaster()) return;
  //G4int nofEvents = aRun->GetNumberOfEvent();
  //if (nofEvents == 0) return;
  if(!IsMaster()) return;
  G4int nofEvents = aRun->GetNumberOfEvent();
  if (nofEvents == 0) return;

  G4cout << "RunActionMaster::EndOfRunAction" << G4endl;
  const G4int rank = G4MPImanager::GetManager()-> GetRank();
 
  G4cout << "=====================================================" << G4endl;
  G4cout << "Start EndOfRunAction for master thread in rank: " << rank<<G4endl;
  G4cout << "=====================================================" << G4endl;
 
  if ( !G4MPImanager::GetManager()->IsExtraWorker() ) {

    //Save histograms *before* MPI merging for rank #0
    if (rank == 0) {
      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->Write();
      analysisManager->CloseFile(false); // close without resetting histograms 
    }


      //Merge of g4analysis objects
      G4cout << "Go to merge histograms " << G4endl;
      G4MPIhistoMerger hm(G4AnalysisManager::Instance());
      hm.SetVerbosity(1);
      hm.Merge();
      G4cout << "Done merge histograms " << G4endl;
    }
 
   //Save g4analysis objects to a file
   //NB: It is important that the save is done *after* MPI-merging of histograms
   //One can save all ranks or just rank0, chane the if  
  if (true){  
  // if (rank == 0){
    auto analysisManager = G4AnalysisManager::Instance();
    if (rank == 0) {
      analysisManager->OpenFile("out_test-merged");  }
    analysisManager->Write();
    analysisManager->CloseFile();  }

   G4cout << "===================================================" << G4endl;
   G4cout << "End EndOfRunAction for master thread in rank: " << rank << G4endl;
   G4cout << "===================================================" << G4endl;


 // auto analysisManager = G4AnalysisManager::Instance();
 // analysisManager->Write();
 // analysisManager->CloseFile();
}
```

### Configuración Clúster

Para configurar el clúster y hacer uso de este con open mpi creamos un archio de texto plano llamado hostfile. Para este caso se tiene disponible 5 computadores con 8 hilos cada uno, es decir el clúster tiene 40 hilos disponibles. Si se desea usar todos los hilos disponibles el archivo debería erse así:

```sh
  master slots=8
  slave1 slots=8
  slave2 slots=8
  slave3 slots=8
  slave4 slots=8
```

Se puede administrar la cantidad de hilos a usar y en que computadores,por ejemplo:

```sh
  master slots=2
  slave1 slots=2
  slave2 slots=2
  slave3 slots=2
  slave4 slots=2
```
En este caso configuramos el clúster para usar 2 hilos por cada computador lo que da un total de 10 hilos a usar por el clúster.


### Comando Ejecución con Open mpi

A continuación el paso a paso para la ejecución de la aplicación:

Cargar variables de entorno:

```sh
$ source ~/Geant4-10.6/install/bin/geant4.sh
$ export PATH=/home/gfnun/sharedFolder/openmpi-install/bin:$PATH
$ export LD_LIBRARY_PATH=/home/gfnun/sharedFolder/openmpi-install/lib:$LD_LIBRARY_PATH 
```
Compilar, para este caso por ejemplo el nombre de la carpeta donde se encuentra el proyecto es g4pntest_mpi:

```sh
$ cd g4pntest_mpi
$ mkdir build
$ cd build
$ cmake -DG4mpi_DIR=/home/gfnun/Geant4-10.6/geant4-mpi/lib/G4mpi-10.6.1 -DCMAKE_CXX_COMPILER=/home/gfnun/sharedFolder/openmpi-install/bin/mpicxx \
      –DGeant4_DIR=/home/gfnun/Geant4-10.6/install/lib/Geant4-10.6.1 ../
$ make -j`nproc`  
```
Ejecución aplicación:

```sh
mpirun -np 40 -hostfile hostfile -x G4ABLADATA -x G4ENSDFSTATEDATA -x G4INCLDATA -x G4LEDATA -x G4LEVELGAMMADATA -x G4NEUTRONHPDATA -x G4PARTICLEXSDATA -x G4PIIDATA -x G4REALSURFACEDATA -x G4SAIDXSDATA -x G4RADIOACTIVEDATA -oversubscribe ./PNtest ../macros/THEmacro.mac
```
Si se requiere usar menos hilos, por ejemplo 12, el comando de ejecución sería así:

```sh
mpirun -np 12 -hostfile hostfile -x G4ABLADATA -x G4ENSDFSTATEDATA -x G4INCLDATA -x G4LEDATA -x G4LEVELGAMMADATA -x G4NEUTRONHPDATA -x G4PARTICLEXSDATA -x G4PIIDATA -x G4REALSURFACEDATA -x G4SAIDXSDATA -x G4RADIOACTIVEDATA -oversubscribe ./PNtest ../macros/THEmacro.mac
```









