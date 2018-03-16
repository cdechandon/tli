
#page de connexion html avec methode post qui renvoie sur verification
<!DOCTYPE HTML>
<HEAD>
<title> page de connexion</title>
</head>
<html>
<p>

    Veuillez rentrez le champ<br />

    Veuillez taper :

</p>


<form action="verification.php" method="post"> <!-- on envoie via le formulaire le pseudo et le mot de passe , par default les valeurs ne s'effacent pas en cliquant => a faire-->

<p>
	<input type="text" name="pseudo" value="Rentrez votre pseudo" /><br/>
	<input type="password" name="mdp" value="Mot de passe"/><br/>

    <input type="submit" value="Valider" />

</p>

</form>
</html>
