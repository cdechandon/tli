-- phpMyAdmin SQL Dump
-- version 4.7.7
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le :  ven. 09 mars 2018 à 14:21
-- Version du serveur :  10.1.30-MariaDB
-- Version de PHP :  7.2.1

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
(0, 'testcrypt', 'testcrypt1', 'coucou@gmail.com', 'testcrypt', '$2y$10$BkhkZFSflmabgagqobevVe7xkFLB63GMmqHRlTI4tGdTCLpf0yoC6'),
(8, 'sangoku', 'valou', 'rahiti@hotmail.fr', 'rahiti', '$2y$10$1Fzr3UMn7eLYOJR9kUHpteWIi0hU41UJ6GseXtinzkW9uW7Ee.kBK'),
(11, 'Andrieux', 'Seraphin', 'craf@gmail.fr', 'craf83', '$2y$10$7fo7w0S9oV2i6.JRxr.qPexfvbOdhLZyDMjyjlgjL3dVayjoZWHw2');

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
  MODIFY `id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=12;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
