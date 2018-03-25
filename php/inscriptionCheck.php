 <?php
//require('model.php');
class Compte
{
    private $_pseudo;
    private $_password;
	private $_mail;
	private $_nom;
	private $_prenom;

    function __construct($pseu, $mdp,$mail,$nom,$prenom)
    {
        $this->_pseudo = $pseu;
        if(isset($mdp)){
		$this->_password = $mdp;}
	else{
		$this->_password ="";
	}
	$this->_mail = $mail;
	$this->_nom = $nom;
	$this->_prenom = $prenom;
    }


	public function verifNom(){
	$verif=false;
	if($this->_nom==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}

	public function verifPrenom(){
	$verif=false;
	if($this->_prenom==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifPseudo(){
	$verif=false;
	if($this->_pseudo==NULL|PseudoExist($this->_pseudo)){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
public function verifModifPseudo(){
		
	return !PseudoExist($this->_pseudo);
	

}
	public function verifMail(){
	if(filter_var($this->_mail, FILTER_VALIDATE_EMAIL) and !MailExist($this->_mail)){
		
	return true;
	}
	else
	{
		
			return false;
		
	}

}
	public function verifPassword(){
	$verif=false;
	if($this->_password==NULL|strlen($this->_password)<7){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}

}



?>
