"""""""""""""""""""""""""""""""""""""""""
" File: glsl.vim
" Author: phreax <m.thomas@networksec.de>
" LastModified: 2011-10-13
" Description: GLSL livecoding plugin
"

" load liveclient extension
pyfile ~/.vim/python/liveclient.py

command! -nargs=0 SendVertShader :python  liveclient.send_shader("shader_vp")
command! -nargs=0 SendFragShader :python  liveclient.send_shader("shader_fp")

" send shader source
au BufWritePost *.frag :SendFragShader
au BufWritePost *.vert :SendVertShader

