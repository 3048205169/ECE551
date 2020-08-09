#include<stdlib.h>
#include<stdio.h>

unsigned power(unsigned x, unsigned y);

void  run_check(unsigned x,unsigned y,unsigned expected_ans)
{
  unsigned ans=1;
  if(y>0)
    {
  for(int i=0;i<y;i++)
    {
      ans=ans*x;
    }
    }
  else if(y=0)
    {
      ans=0;
    }
  if(ans!=expected_ans)
    {
      exit(EXIT_FAILURE);
    }
}


int main()
{
  for(int x=0;x<1000;x++)
    {
      for(int y=0;y<1000;y++)
	{
	  run_check(x,y,power(x,y));
	}
    }
  return EXIT_SUCCESS;  

}
