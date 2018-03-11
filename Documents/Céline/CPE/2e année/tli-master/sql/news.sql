-- phpMyAdmin SQL Dump
-- version 4.7.7
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le :  lun. 26 fév. 2018 à 12:10
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
-- Structure de la table `news`
--

CREATE TABLE `news` (
  `id` int(11) NOT NULL,
  `titre` varchar(100) NOT NULL,
  `contenu` text NOT NULL,
  `date` datetime NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `news`
--

INSERT INTO `news` (`id`, `titre`, `contenu`, `date`) VALUES
(1, 'Ouverture', 'Voici la news d ouverture du site de la team écureuils. Youpiiiiiiiiiiii ', '2018-02-25 17:22:18'),
(2, 'Test', 'Ya plus qu\'a rajouter des news', '2018-02-25 17:45:20'),
(3, 'Info du dimanche', 'Qui est chaud de Smarty ??\r\nflorent Cadot peut etre ? xd', '2018-02-25 19:45:06'),
(4, 'Lundi', 'Camille Dahan a vote rose aux dernieres nouvelles xdddd ', '2018-02-26 00:00:00'),
(5, 'Indien de rahiti', 'Rahiti perrodeau cherche un indien a volonte. Il en a marre de manger seul et na pas peur des hemoroides. Ces amis aussi ? Nous le saurons au prochain episode.\r\nA+ les florizarres.', '2018-02-26 11:00:00'),
(6, 'Flash special: Inceste a Lyon 3', 'D\'apres une source fiable (La future victime), le celebre Rahiti Perodeau va etre victime d\'un tragique evenement. Son pere va passer un mois dans son lit. Florent Cadot lui meme a dit :\"Attention de pas vous choper\".\r\nAvant ou apres l\'Indien, le résultat ne sera pas le même.\r\nLa suite aux prochaines news....', '2018-02-26 11:17:21'),
(7, 'La derniere de la journee', 'Flo et Margue vont peut etre se parler ajd. Flo est comme un dingue. Valou va battre son record de branlette ( 12 ajd).\r\nRahiti apprehende le retour de son pere ... (on peut comprendre).\r\nLes notes de POO a laisse derriere elles un profond desespoir. \r\nPolo explique le role du pole com avec les bleus. Quentin affronte Paul. Il a dit oui oui je le fais.\r\nCamille Dahan a vote rose alz.\r\nLa suite aux prochaines news ;) ', '2018-02-26 10:45:24');

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `news`
--
ALTER TABLE `news`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `news`
--
ALTER TABLE `news`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
