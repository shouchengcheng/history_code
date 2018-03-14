/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
  int add;
  int val1,val2;
  struct ListNode *p,*temp;
  p = malloc(sizeof(struct ListNode));
  temp = p;
  add =  0;
  while((l1!=NULL)||(l2!=NULL)){

    val1=0;
    val2=0;
    if(l1==NULL){
      val2=l2->val;
      l2=l2->next;
    }else if(l2==NULL){
      val1=l1->val;
      l1=l1->next;
    }else{
      val1=l1->val;
      val2=l2->val;
      l1=l1->next;
      l2=l2->next;
    }
    temp->val = (add+val1+val2)%10;
    add = (add+val1+val2)/10;
    if((add!=0)||l1!=NULL||l2!=NULL){
      temp->next = malloc(sizeof(struct ListNode));
      temp = temp->next;
    }else{
      temp->next =NULL;
    }
  }
  if(add){
    temp->next=NULL;
    temp->val = add;
  }
  return p;
}
