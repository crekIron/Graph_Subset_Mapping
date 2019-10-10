	int nextj = 0;
	int p = 1;
	for(int i=1; i<=n; i++){
		if (deg_mail[i-1]==0)
		{
			for(int j=p; j<=m; j++){
				if (deg_phone[j-1]==0)
				{
					clause = clause + get_ind_match(i,j)+ " 0\n";
					p=j+1;
					break;	
				}
			}
		}
	}