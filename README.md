# 🚗 Simulateur de Parking – Projet C (ESIEA)

## 🧩 Présentation

Ce projet est un **jeu de simulation de parking** développé en **C** avec la bibliothèque **SDL3**.  
Le joueur contrôle une voiture dans un parking et doit **la garer à un emplacement précis** tout en **évitant les autres voitures**.

Une fois la voiture garée :
- Une **nouvelle voiture** est attribuée au joueur.

---

## 🎯 Objectifs du jeu

- Contrôler une voiture dans un parking dynamique.
- Trouver et atteindre la place de stationnement indiquée.
- Éviter les collisions avec :
  - les murs du parking ;
  - les voitures stationnées ou en circulation.
- Gérer plusieurs véhicules successivement.

---

## 🧱 Fonctionnalités principales

- **Moteur graphique** basé sur [SDL3](https://github.com/libsdl-org/SDL).
- **Menu interactif** avec boutons (Jouer, Quitter…).
- **Liste chaînée** pour gérer dynamiquement les voitures.
- **Détection de collisions** entre véhicules et obstacles.
- **Modes de jeu** :
  - *Fluide* → peu de voitures.
  - *Chargé* → plus de voitures, trafic plus dense.
- **Boucle de jeu stable à 60 FPS** (~16 ms/frame).

---

## ⚙️ Compilation et exécution

### 🔧 Prérequis

- **Linux**, **macOS** ou **Windows**  
- **CMake ≥ 3.20**
- **SDL3** (voir installation ci-dessous)

### 🧱 Installation de SDL3

#### Sous Linux (Ubuntu/Debian)
```bash
sudo apt install libsdl3-dev
```

#### Sous macOS (Homebrew)
```bash
brew install sdl3
```

#### Sous Windows
Télécharge la version SDL3 depuis :  
👉 [https://github.com/libsdl-org/SDL/releases](https://github.com/libsdl-org/SDL/releases)

Puis ajoute le chemin du SDK à ton projet dans CMake.

---

### 🏗️ Compilation avec CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

---

### ▶️ Exécution

Depuis le dossier `build` :
```bash
./programme
```

## 🕹️ Commandes du joueur

| Touche | Action |
|--------|---------|
| ↑ | Avancer |
| ↓ | Reculer |
| ← | Tourner à gauche |
| → | Tourner à droite |
| Échap | Retour au menu principal |

---

## 💡 Points techniques abordés

- Pointeurs et **allocation dynamique**
- **Listes chaînées**
- **Gestion de fichiers** (textures)
- **Gestion d’événements SDL3**
- **Rendu graphique** (textures, rotations, couleurs)
- **Organisation modulaire** (plusieurs fichiers `.c` / `.h`)
- Compilation avec **CMake**

---

## 👥 Auteurs

Projet réalisé dans le cadre du **cours de Projet C – ESIEA 3A S1 (2025/2026)**  

**Binôme :**
- 🧑‍💻 Neves Sousa Leandro  
- 🧑‍💻 Ouazzani Chahdi Ilyess
- 👩‍💻 Mfumumbele Hortensia

---

## 📜 Licence

Projet académique – reproduction libre à des fins pédagogiques uniquement.  
© 2025 ESIEA — Tous droits réservés.
