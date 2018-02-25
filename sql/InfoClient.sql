-- READ ME : Creer une base de donnee TLI ou alors modifier ici le nom de la base de donnee
-- phpMyAdmin SQL Dump
-- version 4.7.6
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le :  ven. 16 fév. 2018 à 14:15
-- Version du serveur :  10.1.29-MariaDB
-- Version de PHP :  7.2.0

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `TLI`
--

-- --------------------------------------------------------

--
-- Structure de la table `InfoClient`
--

CREATE TABLE `InfoClient` (
  `id` int(255) NOT NULL,
  `Nom` varchar(255) NOT NULL,
  `Prenom` varchar(255) NOT NULL,
  `mail` varchar(255) NOT NULL,
  `pseudo` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `InfoClient`
--

INSERT INTO `InfoClient` (`id`, `Nom`, `Prenom`, `mail`, `pseudo`, `password`) VALUES
(1, 'Admin', 'istrateur', 'admin@hotmail.fr', 'administrator', 'admin'),
(6, 'Bart', 'Tit', 'titbgdu69@gmail.com', 'titbgdu69', 'titbgdu69');

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `InfoClient`
--
ALTER TABLE `InfoClient`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `InfoClient`
--
ALTER TABLE `InfoClient`
  MODIFY `id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
