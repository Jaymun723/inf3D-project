# Projet INF3D

Pour commencer :
```sh
git clone --recurse-submodules https://github.com/Jaymun723/inf3D-project.git
```

## Ce qu'il me reste à faire (Caro Edition)

*Partie : optimisation*

1. ~~Finir le chunk manager~~
2. Optimisation du rendu des mesh : au lieu que la mesh contienne tous les cubes, seuls les faces visibles doivent être envoyées

*Partie : rendre le truc utilisable*

3. ~~Caméra bougeable: personnage avec de la "physique" ? caméra libre qui peut tout explorer ?~~

*Partie : rendre le truc joli*

4. Faire des blocs joli, mat, brillant
5. Skybox 

*Partie : rendre le truc joli, si on le temps*

6. Faire des blocs source lumineuse (tahh les blocs de glowstone de Minecraft)
7. Faire de la végétation, du feuillage

## Documentation

Pour le rendu il faut faire un "Voxel engine", en gros un truc qui affiche beaucoup de cubes efficacement (genre Minecraft).
Pour l'instant je suis ce tuto :
- https://sites.google.com/site/letsmakeavoxelengine/home/

Ensuite il faudra faire notre génération de terrain :
- Chaîne de Markov : https://github.com/mxgmn/MarkovJunior
- Wave Function Collapse : https://github.com/mxgmn/WaveFunctionCollapse