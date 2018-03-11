 <?php $password= password_hash($_POST['password'], PASSWORD_DEFAULT); 
	$nom= $_POST['nom']; // on creer des variables qui contiennent les argument via la methode post
	$prenom= $_POST['prenom'];
	$pseudo= $_POST['pseudo'];
	$mail= $_POST['mail'];
if(isset($password)&isset($pseudo)&isset($mail)&isset($nom)&isset($prenom)){ // verification de l'existance de ces variables
	

	try
{
	$bdd = new PDO('mysql:host=localhost;dbname=TLI;charset=utf8', 'root', '');// on accede a la base de donnee
}
catch(Exception $e)
{
        die('Erreur : '.$e->getMessage());// erreur affichee (on croise les doigts)
}



$req = $bdd->prepare('INSERT INTO InfoClient(Nom, Prenom,mail, pseudo, password) VALUES(:Nom, :Prenom ,:mail,:pseudo,  :password)');

$req->execute(array(

     	'Nom' => $nom,
	'Prenom' => $prenom,
	'mail' => $mail,
    	'pseudo' => $pseudo,    
   	'password' => $password,

    ));


echo 'Vous etes inscrit';

	//header('Location: site.html');


}
else{
echo "Sorry you are not ready for this adventure";
}
