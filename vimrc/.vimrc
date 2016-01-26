set runtimepath+=~/.vim_runtime

source ~/.vim_runtime/vimrcs/basic.vim
source ~/.vim_runtime/vimrcs/filetypes.vim
source ~/.vim_runtime/vimrcs/plugins_config.vim
source ~/.vim_runtime/vimrcs/extended.vim

try
source ~/.vim_runtime/my_configs.vim
catch
endtry

set autoindent 
set cindent
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1

let g:DoxygenToolkit_briefTag_pre="LovePrincessStudio  "
let g:DoxygenToolkit_paramTag_pre="@param  "
let g:DoxygenToolkit_returnTag="@returns   "
let g:DoxygenToolkit_blockHeader="-----------------------LovePrincessStudio-------------------------------"
let g:DoxygenToolkit_blockFooter="-----------------------LovePrincessStudio-------------------------------"
let g:DoxygenToolkit_authorName="Pengfei Gao\<enter>* @emailnet.aifei8@gmail.com\<enter>* @weiChat Moira_-\<enter>* @phone number:15526773247\<enter>* @QQ:2466557844\<enter>* @blog:http://blog.aifei8.net\<enter>* @Copyright(C) 2013-2016 aifei8.net All right reserved "
let g:DoxygenToolkit_licenseTag="GPL 2.0"
 
 
 
let s:licenseTag = "Copyright(C) 2013-2016 Love Princess Studio\<enter>"
let s:licenseTag = s:licenseTag . "All right reserved\<enter>"
let g:DoxygenToolkit_licenseTag = s:licenseTag
let g:DoxygenToolkit_briefTag_funcName="yes"
let g:doxygen_enhanced_color=1
 
 
vmap <C-S-P>    dO#endif<Esc>PO#if 0<Esc>
map <F4> <Esc>:Dox<cr>
map <F5> <Esc>:DoxAuthor<cr>
map <F6> <Esc>:DoxLic<cr>
map <F7> <Esc>:Tlist<cr>
