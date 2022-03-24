<html>
	<head>
		<title>
			Results of GenClass
		</title>
		<link rel="stylesheet" href="./highlight/styles/default.css">
		<script src="./highlight/highlight.pack.js"></script>
		<script>hljs.initHighlightingOnLoad();</script>
	</head>
	<body>
	<?php
		include_once("mysql.php");
		$link=mysqli_connect($dbhost,$dbuser,$dbpass);
		mysqli_select_db($link,$database);
		$sql="select name from genclass group by name";
		$res=mysqli_query($link,$sql);
		echo "<table border=1>";
		echo "<h4>Output form</h4><form method=post action=$php_self>";
		echo "<tr>";
		echo "<td>Problem name:</td><td align=right><select name=problem>";
		while($row=mysqli_fetch_array($res,MYSQLI_ASSOC))
			echo "<option>".$row["name"]."</option>";
		echo "</select></td></tr><tr>";
		echo "<input type=hidden name=flag value=yes>";
		echo "<td></td><td align=right><input type=submit value=show></td></tr>";
		echo "</form>";
		echo "</table>";
		echo "</td>";
		echo "<td>";
		if(!empty($_POST["problemid"]))
		{
			$sql="delete from genclass where id=".$_POST["problemid"];
			$res=mysqli_query($link,$sql);
		}
		if(!empty($_POST["flag"]))
		{
			echo "<h3 align=center> Output for ".$_POST["problem"]."</h3>";
			echo "<hr>";
			$name=$_POST["problem"];
			
			$sql="select * from genclass where name='$name' order by trainerror";
			$res=mysqli_query($link,$sql);
			echo "<table border=1 align=center width=40%>";
			echo "<tr>";
			echo "<th> Test Error </th>";
			echo "<th> Function </th>";
			echo "</tr>";
			$total=0.0;
			$icount=0;
			while($row=mysqli_fetch_array($res,MYSQLI_ASSOC))
			{
				$testerror=$row["testerror"];
				$total+=$testerror;
				$icount++;
				$bestprogram=htmlspecialchars($row["bestprogram"]);
				echo "<tr>";
				echo "<td align=right width=20%>";
					echo $testerror."%";
				echo "</td>";
				echo "<td align=left>";
					echo "<pre><code class=\"c++\">";
					echo $bestprogram;
					echo "</code></pre>";
				echo "</td>";
				echo "<td align=center>";
				echo "<form method=post action=$php_self>";
				echo "<input type=hidden name=problemid value=".$row["id"]." />";
				echo "<input type=hidden name=flag value=yes>";	
				echo "<input type=hidden name=problem value=$name>";
				echo "<input type=submit value=delete>";
				echo "</form>";
				echo "</td>";
				echo "</tr>";
			}
			echo "</table>";	
			$avgerror=$total/$icount;
			echo "<h4> Total number of runs: $icount Average number of error: $avgerror% </h4>";
			echo "</hr>";
		}
		mysqli_close($link);
	?>
	</body>
</html>
