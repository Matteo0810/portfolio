-- Création du schema
DROP SCHEMA IF EXISTS transmusicales CASCADE; -- on supprime le schema si il existe déjà
CREATE SCHEMA transmusicales;
SET SCHEMA 'transmusicales';

-- Table _formation
CREATE TABLE _formation (
    libelle_formation VARCHAR(100) NOT NULL,
    CONSTRAINT libelle_formation_pk PRIMARY KEY (libelle_formation)
);

-- Table _pays
CREATE TABLE _pays (
	nom_p VARCHAR(50) NOT NULL,
	CONSTRAINT nom_p_pk PRIMARY KEY (nom_p)
);

-- Table _type_musique
CREATE TABLE _type_musique (
	type_m VARCHAR(50) NOT NULL,
	CONSTRAINT type_m_pk PRIMARY KEY (type_m)
);

-- Table _groupe_artiste
CREATE TABLE _groupe_artiste (
	id_groupe_artiste CHAR(3) NOT NULL,
	nom_groupe_artiste VARCHAR(50) NOT NULL,
	site_web VARCHAR(100),
	debut INT NOT NULL,
	sortie_discographie INT NOT NULL,
	formation VARCHAR(200) NOT NULL,
	type_principal VARCHAR(50) NOT NULL,
	type_ponctuel VARCHAR(50) NOT NULL,
	origine VARCHAR(50) NOT NULL,
	CONSTRAINT id_groupe_artiste_pk PRIMARY KEY (id_groupe_artiste),
  CONSTRAINT _groupe_artiste_fk_formation FOREIGN KEY (formation) REFERENCES _formation(libelle_formation),
  CONSTRAINT _groupe_artiste_fk_type_musique_1 FOREIGN KEY (type_principal) REFERENCES _type_musique(type_m),
  CONSTRAINT _groupe_artiste_fk_type_musique_2 FOREIGN KEY (type_ponctuel) REFERENCES _type_musique(type_m),
  CONSTRAINT _groupe_artiste_fk_pays FOREIGN KEY (origine) REFERENCES _pays(nom_p)
);

-- Table _annee
CREATE TABLE _annee (
	an int NOT NULL,
	CONSTRAINT an_pk PRIMARY KEY (an)
);

-- Ajout des contraintes à groupe artiste grâce à la table année
ALTER TABLE _groupe_artiste  
  ADD CONSTRAINT _groupe_artiste_fk_annee_1 FOREIGN KEY (debut) REFERENCES _annee(an);
ALTER TABLE _groupe_artiste 
   ADD CONSTRAINT _groupe_artiste_fk_annee_2 FOREIGN KEY (sortie_discographie) REFERENCES _annee(an);

-- Table _edition
CREATE TABLE _edition (
	nom_edition VARCHAR(50) NOT NULL,
	annee_edition INT NOT NULL,
	CONSTRAINT edition_pk PRIMARY KEY (nom_edition),
  CONSTRAINT _edition_fk_annee FOREIGN KEY (annee_edition) REFERENCES _annee(an)
);

-- Table _lieu
CREATE TABLE _lieu (
	id_lieu CHAR(3) NOT NULL,
	nom_lieu VARCHAR(50) NOT NULL,
	acces_pmr BOOLEAN NOT NULL,
	capacite_max INT NOT NULL,
	type_lieu VARCHAR(50) NOT NULL,
	est_dans VARCHAR(50) NOT NULL,
	CONSTRAINT _id_lieu_pk PRIMARY KEY (id_lieu)
);

-- Table _ville
CREATE TABLE _ville (
	nom_v VARCHAR(50) NOT NULL,
	se_situe VARCHAR(50) NOT NULL,
	CONSTRAINT nom_v_pk PRIMARY KEY (nom_v),
  CONSTRAINT _ville_fk_pays FOREIGN KEY (se_situe) REFERENCES _pays(nom_p)
);

-- Ajout de la contrainte sur la ville dans la table lieu
ALTER TABLE _lieu
    ADD CONSTRAINT _lieu_fk_ville FOREIGN KEY (est_dans) REFERENCES _ville(nom_v);

-- Table _representation
CREATE TABLE _representation (
	numero_representation CHAR(3) NOT NULL,
	heure CHAR(4) NOT NULL,
	date_representation DATE NOT NULL,
	jouee_par CHAR(3) NOT NULL,
	a_lieu_dans CHAR(3) NOT NULL,
	concert_jouee CHAR(3) NOT NULL,
	CONSTRAINT numero_representation_pk PRIMARY KEY (numero_representation),
	CONSTRAINT _representation_fk_groupe_artiste FOREIGN KEY (jouee_par) REFERENCES _groupe_artiste(id_groupe_artiste),
	CONSTRAINT _representation_fk_lieu FOREIGN KEY (a_lieu_dans) REFERENCES _lieu(id_lieu)
);

-- Table _concert
CREATE TABLE _concert (
	no_concert CHAR(3) NOT NULL,
	titre VARCHAR(50) NOT NULL,
	resume TEXT,
	duree INT NOT NULL,
	tarif FLOAT NOT NULL,
	genre VARCHAR(50) NOT NULL,
	se_deroule VARCHAR(50) NOT NULL,
	CONSTRAINT no_concert_pk PRIMARY KEY (no_concert),
	CONSTRAINT _concert_fk_type_musique FOREIGN KEY (genre) REFERENCES _type_musique(type_m),
	CONSTRAINT _concert_fk_edition FOREIGN KEY (se_deroule) REFERENCES _edition(nom_edition)
);

-- Ajout de la contraintesur le concert dans la table représentation
ALTER TABLE _representation 
  ADD CONSTRAINT _representation_fk_concert FOREIGN KEY (concert_jouee) REFERENCES _concert(no_concert);
