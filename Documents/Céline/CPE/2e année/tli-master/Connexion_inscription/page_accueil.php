<?php
session_start();
if (isset($_SESSION['id']) AND isset($_SESSION['pseudo']))

{

    echo 'Bonjour ' . $_SESSION['pseudo'];

}
else {
echo 'pas connecter';
}
?>
<!DOCTYPE HTML>
<!DOCTYPE html>
<html>
<head>
	<title>Projet TLI</title>
</head>
<body>
	<p>Bienvenue a toutes et tous sur le site de la team ecureuil
	</p>
	<br>
	<p>
		Mais avant de profiter du contenu, je vous laisse <lis>
			<ul>soit vous connecter : <a href=connexion.php>connexion</a>
			<ul>soit vous inscrire : <a href=inscription.php>Inscription</a>


	</p>

</body>
</html>
